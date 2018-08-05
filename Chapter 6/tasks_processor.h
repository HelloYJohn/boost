#include <boost/asio/io_service.hpp>

namespace detail {
    class tasks_processor : private boost::noncopyable {
        boost::asio::io_service ios_;
        boost::asio::io_service::work work_;

        tasks_processor()
            :ios_(), work_(ios_) {
        }

      public:
        static tasks_processor& get() {
            static tasks_processor tasks_processor_obj;
            return tasks_processor_obj;
        }

        template<class T>
        inline void push_back(const T& task_unwrapped) {
            ios_.post(detail::make_task_wrapped(task_unwrapped));
        }

        void start() {
            ios_.run();
        }

        void stop() {
            ios_.stop();
        }
    };
};
