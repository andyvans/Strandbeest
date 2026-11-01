#include "video_stream.h"
#include <esp_camera.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <cstring>
#include <cstdio>
#include "constants.h"

extern AsyncWebServer server;

namespace {

    const char kMjpegTrailer[] = "\r\n";
    constexpr size_t kMjpegHeaderMaxLen = 96;

    class MjpegStreamResponse : public AsyncAbstractResponse {
    public:
        MjpegStreamResponse() {
            _code = 200;
            _contentType = "multipart/x-mixed-replace; boundary=frame";
            _sendContentLength = false;
            _chunked = true;
            addHeader("Cache-Control", "no-cache");
            addHeader("Pragma", "no-cache");
        }

        ~MjpegStreamResponse() override {
            if (fb_ != nullptr) {
                esp_camera_fb_return(fb_);
                fb_ = nullptr;
            }
        }

        bool _sourceValid() const override {
            return true;
        }

        size_t _fillBuffer(uint8_t* buf, size_t maxLen) override {
            if (maxLen == 0) {
                return 0;
            }

            size_t written = 0;

            while (written < maxLen) {
                if (fb_ == nullptr) {
                    fb_ = esp_camera_fb_get();
                    if (fb_ == nullptr) {
                        return written > 0 ? written : RESPONSE_TRY_AGAIN;
                    }

                    headerLen_ = std::snprintf(
                        header_,
                        sizeof(header_),
                        "--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n",
                        static_cast<unsigned int>(fb_->len));
                    if (headerLen_ <= 0) {
                        esp_camera_fb_return(fb_);
                        fb_ = nullptr;
                        return written > 0 ? written : RESPONSE_TRY_AGAIN;
                    }
                    headerIndex_ = 0;
                    frameIndex_ = 0;
                    trailerIndex_ = 0;
                }

                if (headerIndex_ < static_cast<size_t>(headerLen_)) {
                    const size_t headerRemaining = static_cast<size_t>(headerLen_) - headerIndex_;
                    const size_t toCopy = std::min(maxLen - written, headerRemaining);
                    std::memcpy(buf + written, header_ + headerIndex_, toCopy);
                    written += toCopy;
                    headerIndex_ += toCopy;
                    continue;
                }

                if (frameIndex_ < fb_->len) {
                    const size_t frameRemaining = fb_->len - frameIndex_;
                    const size_t toCopy = std::min(maxLen - written, frameRemaining);
                    std::memcpy(buf + written, fb_->buf + frameIndex_, toCopy);
                    written += toCopy;
                    frameIndex_ += toCopy;
                    continue;
                }

                if (trailerIndex_ < sizeof(kMjpegTrailer) - 1) {
                    const size_t trailerRemaining = (sizeof(kMjpegTrailer) - 1) - trailerIndex_;
                    const size_t toCopy = std::min(maxLen - written, trailerRemaining);
                    std::memcpy(buf + written, kMjpegTrailer + trailerIndex_, toCopy);
                    written += toCopy;
                    trailerIndex_ += toCopy;
                    continue;
                }

                esp_camera_fb_return(fb_);
                fb_ = nullptr;
            }

            return written;
        }

    private:
        camera_fb_t* fb_ = nullptr;
        char header_[kMjpegHeaderMaxLen] = {};
        int headerLen_ = 0;
        size_t headerIndex_ = 0;
        size_t frameIndex_ = 0;
        size_t trailerIndex_ = 0;
    };

} // namespace

void setupCamera() {
    camera_config_t config = {};
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = constants::CAM_PIN_D0;
    config.pin_d1 = constants::CAM_PIN_D1;
    config.pin_d2 = constants::CAM_PIN_D2;
    config.pin_d3 = constants::CAM_PIN_D3;
    config.pin_d4 = constants::CAM_PIN_D4;
    config.pin_d5 = constants::CAM_PIN_D5;
    config.pin_d6 = constants::CAM_PIN_D6;
    config.pin_d7 = constants::CAM_PIN_D7;
    config.pin_xclk = constants::CAM_PIN_XCLK;
    config.pin_pclk = constants::CAM_PIN_PCLK;
    config.pin_vsync = constants::CAM_PIN_VSYNC;
    config.pin_href = constants::CAM_PIN_HREF;
    config.pin_sccb_sda = constants::CAM_PIN_SCCB_SDA;
    config.pin_sccb_scl = constants::CAM_PIN_SCCB_SCL;
    config.pin_pwdn = constants::CAM_PIN_PWDN;
    config.pin_reset = constants::CAM_PIN_RESET;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 18;
    config.fb_count = 3;
    config.grab_mode = CAMERA_GRAB_LATEST;
    config.fb_location = CAMERA_FB_IN_PSRAM;

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x\n", err);
        return;
    }

    server.on("/stream", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(new MjpegStreamResponse());
        });
}
