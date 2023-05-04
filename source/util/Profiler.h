//
// Created by Alienso on 27.4.2023..
//

#ifndef GRASSTERRAIN_PROFILER_H
#define GRASSTERRAIN_PROFILER_H


#include <chrono>
#include <iostream>
#include <fstream>
#include <algorithm>

#define PROFILING 0
#if PROFILING
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name);
#else
#define PROFILE_SCOPE(name)
#endif

struct ProfileResult{

    ProfileResult(std::string _name, long long _start, long long _end) : name(_name), start(_start), end(_end){}
    std::string name;
    long long start,end;
};

struct InstrumentationSession{

    InstrumentationSession(const std::string& _name) : name(_name){}
    std::string name;
};

class Instrumentor {
private:
    InstrumentationSession* currentSession;
    std::ofstream outputStream;
    int profileCount;

public:
    Instrumentor() : currentSession(nullptr), profileCount(0) {}

    void beginSession(const std::string& name, const std::string& filepath = "results.json")
    {
        outputStream.open(filepath);
        writeHeader();
        currentSession = new InstrumentationSession{ name };
    }

    void endSession()
    {
        writeFooter();
        outputStream.close();
        delete currentSession;
        currentSession = nullptr;
        profileCount = 0;
    }

    void writeProfile(const ProfileResult& result)
    {
        if (profileCount++ > 0)
            outputStream << ",";

        std::string name = result.name;
        std::replace(name.begin(), name.end(), '"', '\'');

        outputStream << "{";
        outputStream << "\"cat\":\"function\",";
        outputStream << "\"dur\":" << (result.end - result.start) << ',';
        outputStream << "\"name\":\"" << name << "\",";
        outputStream << "\"ph\":\"X\",";
        outputStream << "\"pid\":0,";
        outputStream << "\"tid\":" << 0 << ",";
        outputStream << "\"ts\":" << result.start;
        outputStream << "}";

        outputStream.flush();
    }

    void writeHeader()
    {
        outputStream << "{\"otherData\": {},\"traceEvents\":[";
        outputStream.flush();
    }

    void writeFooter()
    {
        outputStream << "]}";
        outputStream.flush();
    }

    static Instrumentor& get()
    {
        static Instrumentor instance;
        return instance;

    }

};

class InstrumentationTimer {
public:
    InstrumentationTimer(const char* _name) : name(_name), stopped(false) {
        startTimePoint = std::chrono::steady_clock::now();
    }

    ~InstrumentationTimer(){
        if (!stopped)
            stop();
    }

    void stop(){
        auto endTimePoint = std::chrono::steady_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimePoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

        Instrumentor::get().writeProfile({name,start,end});

        stopped = true;
    }

private:
    const char* name;
    std::chrono::time_point<std::chrono::steady_clock> startTimePoint;
    bool stopped;
};


#endif //GRASSTERRAIN_PROFILER_H
