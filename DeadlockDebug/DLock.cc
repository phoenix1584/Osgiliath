/*
 * Copyright (c) 2018
 * Author: phoenix1584
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

// WARNING: The code is creating a deadlock on purpose to allow testing. DONOT try this at home !
// Suggested usage : valgrind --tool=helgrind ./DLock

#include <iostream>
#include <thread>
#include <mutex>

std::mutex mutex_1, mutex_2;

void Thread_1(){
    std::lock_guard<std::mutex> l_m2(mutex_2);
    std::cout << "Thread_1\n";
    std::lock_guard<std::mutex> l_m1(mutex_1);
}

void Thread_2(){
    std::lock_guard<std::mutex> l_m1(mutex_1);
    std::cout << "Thread_2\n";
    std::lock_guard<std::mutex> l_m2(mutex_2);
}

void Launcher(){
    std::thread t1(Thread_1);
    std::thread t2(Thread_2);
    t1.join();
    t2.join();
    std::cout << "Finished.\n";
}

int main(){
    Launcher();
    return 0;
}
