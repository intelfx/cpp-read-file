#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <algorithm>
#include <iterator>
#include <string>
#include <cstdio>
#include <cerrno>

#define NONIUS_RUNNER
#include <nonius/nonius.h++>
#include <nonius/main.h++>

NONIUS_PARAM(path, std::string{"read_file.txt"})

std::string read_file_stdio(const char *filename)
{
	std::FILE *fp = std::fopen(filename, "rb");
	if (fp)
	{
		std::string contents;
		std::fseek(fp, 0, SEEK_END);
		contents.resize(std::ftell(fp));
		std::rewind(fp);
		std::fread(&contents[0], 1, contents.size(), fp);
		std::fclose(fp);
		return contents;
	}
	throw errno;
}
NONIUS_BENCHMARK("read_file_stdio", [](nonius::chronometer meter) {
	auto file_path = meter.param<path>();
	meter.measure([&](){ return read_file_stdio(file_path.c_str()); });
})

std::string read_file_stream(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], (std::streamsize)contents.size());
		in.close();
		return contents;
	}
	throw errno;
}
NONIUS_BENCHMARK("read_file_stream", [](nonius::chronometer meter) {
	auto file_path = meter.param<path>();
	meter.measure([&](){ return read_file_stream(file_path.c_str()); });
})

#if 0
std::string read_file_iter(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
	}
	throw errno;
}
NONIUS_BENCHMARK("read_file_iter", [](nonius::chronometer meter) {
	auto file_path = meter.param<path>();
	meter.measure([&](){ return read_file_iter(file_path.c_str()); });
})

std::string read_file_iter_assign(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.reserve(in.tellg());
		in.seekg(0, std::ios::beg);
		contents.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
		in.close();
		return contents;
	}
	throw errno;
}
NONIUS_BENCHMARK("read_file_iter_assign", [](nonius::chronometer meter) {
	auto file_path = meter.param<path>();
	meter.measure([&](){ return read_file_iter_assign(file_path.c_str()); });
})

std::string read_file_iter_copy(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.reserve(in.tellg());
		in.seekg(0, std::ios::beg);
		std::copy(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>(), std::back_inserter(contents));
		in.close();
		return contents;
	}
	throw errno;
}
NONIUS_BENCHMARK("read_file_iter_copy", [](nonius::chronometer meter) {
	auto file_path = meter.param<path>();
	meter.measure([&](){ return read_file_iter_copy(file_path.c_str()); });
})
#endif

/*
 * matches or slightly exceeds raw stdio/iostream performance
 */
std::string read_file_rdbuf(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::ostringstream contents;
		contents << in.rdbuf();
		in.close();
		return contents.str();
	}
	throw errno;
}
NONIUS_BENCHMARK("read_file_rdbuf", [](nonius::chronometer meter) {
	auto file_path = meter.param<path>();
	meter.measure([&](){ return read_file_rdbuf(file_path.c_str()); });
})

/*
 * best in C++20 or newer
 */
std::string read_file_rdbuf_move(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		return (std::ostringstream() << in.rdbuf()).str();
	}
	throw errno;
}
NONIUS_BENCHMARK("read_file_rdbuf_move", [](nonius::chronometer meter) {
	auto file_path = meter.param<path>();
	meter.measure([&](){ return read_file_rdbuf_move(file_path.c_str()); });
})
