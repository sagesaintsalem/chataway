#pragma once
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <thread>
using std::thread;
#include <string>
using std::string;
#include <boost\asio.hpp>
using boost::asio::ip::tcp;
using namespace boost::asio;
#include <boost/asio/ssl.hpp>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <chrono>
#include <ctime>
#include <Windows.h>