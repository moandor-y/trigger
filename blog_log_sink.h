#ifndef PLUGINS_TRIGGER_BLOG_LOG_SINK_H_
#define PLUGINS_TRIGGER_BLOG_LOG_SINK_H_

#include "obs-module.h"
#include "third_party/abseil-cpp/absl/log/log_sink.h"

namespace trigger {

class BlogLogSink : public absl::LogSink {
 public:
  void Send(const absl::LogEntry& entry) override {
    blog(LOG_INFO, "%s", std::string(entry.text_message_with_prefix()).c_str());
  }
};

}  // namespace trigger

#endif  // PLUGINS_TRIGGER_BLOG_LOG_SINK_H_
