#include <iostream>
#include "task_wrapped.h"
#include "tasks_processor.h"
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>

int g_val = 0;
boost::mutex g_val_mutex;

void func_test() {
    {
        // boost::lock_guard<boost::mutex> lock(g_val_mutex);
        ++ g_val;
        std::cout << "g_val value : " << g_val << std::endl;
    }
    if (g_val == 3) {
        throw std::logic_error("Just checking");
    }

    boost::this_thread::interruption_point();

    if (g_val == 10) {
        throw boost::thread_interrupted();
    }

    if (g_val == 90) {
        detail::tasks_processor::get().stop();
    }
}

void func_test_2(int i) {
   std::cout << "func_test_2 : " << i << std::endl; 
}

int main() {
    static const std::size_t tasks_count = 100;

    BOOST_STATIC_ASSERT(tasks_count > 90);

    for(std::size_t i = 0; i < tasks_count; ++i) {
        detail::tasks_processor::get().push_back(&func_test);
    }
    detail::tasks_processor::get().push_back(boost::bind(&func_test_2, 2));

    detail::tasks_processor::get().push_back(boost::bind(std::plus<int>(), 2, 2));

    assert(g_val == 0);

    detail::tasks_processor::get().start();
    
    assert(g_val == 90);
}
