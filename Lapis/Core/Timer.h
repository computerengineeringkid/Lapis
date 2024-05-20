#pragma once
#include <chrono>
#include <iostream>

class Timer {
public:
    Timer()
        : m_Started(false), m_StartTime(), m_EndTime(), m_LastTick() {}

    void Start() {
        m_StartTime = std::chrono::high_resolution_clock::now();
        m_LastTick = m_StartTime;
        m_Started = true;
    }

    void Stop() {
        m_EndTime = std::chrono::high_resolution_clock::now();
        m_Started = false;
    }

    void Reset() {
        m_Started = false;
        m_StartTime = std::chrono::time_point<std::chrono::high_resolution_clock>();
        m_EndTime = std::chrono::time_point<std::chrono::high_resolution_clock>();
        m_LastTick = std::chrono::time_point<std::chrono::high_resolution_clock>();
    }

    double Elapsed() const {
        if (!m_Started) {
            return std::chrono::duration_cast<std::chrono::duration<double>>(m_EndTime - m_StartTime).count();
        }
        return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - m_StartTime).count();
    }

    double Tick() {
        if (!m_Started) {
            return 0.0; // No ticking if timer isn't started.
        }
        auto now = std::chrono::high_resolution_clock::now();
        double delta = std::chrono::duration_cast<std::chrono::duration<double>>(now - m_LastTick).count();
        m_LastTick = now;
        return delta;
    }

private:
    bool m_Started;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_EndTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_LastTick;
};
