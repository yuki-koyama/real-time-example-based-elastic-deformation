// Original source code by Kenshi Takayama

#pragma once

#include <ctime>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdio>
#include "getTime.h"

namespace Y {
    
    // "with precision" means that they use gettimeofday instead of clock(), so they can measuer smaller time than tick time.
    class ClockStartWithPrecision;
    
    class ClockDataWithPrecision {
    private:
        friend class ClockStartWithPrecision;
        std::vector<unsigned long int>  ticks_;
        std::string                     clkName_;
        std::vector<std::string>        names_;
        bool                            showPercent_;
    public:
        ClockDataWithPrecision(const std::string& clkName = "", bool showPercent = true)
        : clkName_(clkName)
        , showPercent_(showPercent)
        {}
        void setClkName(const std::string& clkName) {
            clkName_ = clkName;
        }
        std::string getClkName() const {
            return clkName_;
        }
        void addEntry(const std::string& name) {
            names_.push_back(name);
            ticks_.push_back(0);
        }
        void resetTick() {
            for (std::vector<unsigned long int>::iterator it = ticks_.begin(); it != ticks_.end(); ++ it)
                *it = 0;
        }
        unsigned long int getTick(int index) const { return ticks_[index]; }
        
        unsigned long int getTotal(void) const {
            unsigned long int total = 0;
            for (int i = 0; i < ticks_.size(); ++ i) {
                total += ticks_[i];
            }
            return total;
        }
        
        std::string getName(int index) const { return names_[index]; }
        
        void setShowPercent(bool showPercent) {
            showPercent_ = showPercent;
        }
        void print() {
            double total = 0.0;
            for (size_t i = 0; i < ticks_.size(); ++i)
                total += static_cast<double>(ticks_[i]);
            std::cout << "[";
            if (!clkName_.empty())
                std::cout << clkName_ << " | ";
            for (size_t i = 0; i < ticks_.size(); ++i) {
                std::cout << names_[i] << ":";
                if (showPercent_) {
                    double percent = total == 0.0 ? 0.0 : (100 * ticks_[i] / total);
                    std::cout << std::setprecision(3) << percent << "%, ";
                } else {
                    double t = ((double)ticks_[i]) * 0.0010;
                    printf(" %.3f msec, ", t);
                }
            }
            double t = ((double)total) * 0.0010;
            printf("total: %.3f msec]\n", t);
        }
    };
    
    class ClockStartWithPrecision
    {
        ClockDataWithPrecision* clkData_;
        int currentIndex_;
        bool autoPrint_;
        clock_t clk_;
    public:
        ClockStartWithPrecision(ClockDataWithPrecision* clkData, int currentIndex = 0, bool autoPrint = false, bool resetTick = false)
        : clkData_(clkData)
        , currentIndex_(currentIndex)
        , autoPrint_(autoPrint)
        , clk_(clock())
        {
            if (autoPrint_)
                std::cout << "clock started: " << clkData_->getClkName() << std::endl;
            if (resetTick)
                clkData_->resetTick();
        }
        void setCurrentIndex(int index) {
            unsigned long int clk_old = clk_;
            clk_ = getTimeInUnsignedLongInt();
            clkData_->ticks_[currentIndex_] += clk_ - clk_old;
            currentIndex_ = index;
        }
        void print() {
            unsigned long int clk_old = clk_;
            clk_ = getTimeInUnsignedLongInt();
            clkData_->ticks_[currentIndex_] += clk_ - clk_old;
            clkData_->print();
        }
        ~ClockStartWithPrecision() {
            unsigned long int clk_old = clk_;
            clk_ = getTimeInUnsignedLongInt();
            clkData_->ticks_[currentIndex_] += clk_ - clk_old;
            if (autoPrint_) {
                clkData_->print();
                std::cout << "clock stopped: " << clkData_->getClkName() << std::endl;
            }
        }
    };

    class ClockStart; // used in ClockData

    class ClockData { // used in ClockStart
    private:
        friend class ClockStart;
        std::vector<clock_t    >    ticks_;
        std::string                 clkName_;
        std::vector<std::string>    names_;
        bool                        showPercent_;
    public:
        ClockData(const std::string& clkName = "", bool showPercent = true)
        : clkName_(clkName)
        , showPercent_(showPercent)
        {}
        void setClkName(const std::string& clkName) {
            clkName_ = clkName;
        }
        std::string getClkName() const {
            return clkName_;
        }
        void addEntry(const std::string& name) {
            names_.push_back(name);
            ticks_.push_back(0);
        }
        void resetTick() {
            for (std::vector<clock_t>::iterator it = ticks_.begin(); it != ticks_.end(); ++it)
                *it = 0;
        }
        clock_t     getTick(int index) const { return ticks_[index]; }
        
        clock_t getTotal(void) const {
            int total = 0;
            for (int i = 0; i < ticks_.size(); ++ i) {
                total += ticks_[i];
            }
            return total;
        }
        
        std::string getName(int index) const { return names_[index]; }
        
        void setShowPercent(bool showPercent) {
            showPercent_ = showPercent;
        }
        void print() {
            double total = 0.0;
            for (size_t i = 0; i < ticks_.size(); ++i)
                total += static_cast<double>(ticks_[i]);
            std::cout << "[";
            if (!clkName_.empty())
                std::cout << clkName_ << " | ";
            for (size_t i = 0; i < ticks_.size(); ++i) {
                std::cout << names_[i] << ":";
                if (showPercent_) {
                    double percent = total == 0.0 ? 0.0 : (100 * ticks_[i] / total);
                    std::cout << std::setprecision(3) << percent << "%, ";
                } else {
                    double t = 1000 * ticks_[i] / static_cast<double>(CLOCKS_PER_SEC);
                    printf(" %.3f msec, ", t);
//                    std::cout << (1000 * ticks_[i] / static_cast<double>(CLOCKS_PER_SEC)) << "msec, ";
                }
            }
            std::cout << "total:" << (1000 * total / CLOCKS_PER_SEC) << "msec]\n";
        }
    };
    
    class ClockStart
    {
        ClockData* clkData_;
        int currentIndex_;
        bool autoPrint_;
        clock_t clk_;
    public:
        ClockStart(ClockData* clkData, int currentIndex = 0, bool autoPrint = false, bool resetTick = false)
        : clkData_(clkData)
        , currentIndex_(currentIndex)
        , autoPrint_(autoPrint)
        , clk_(clock())
        {
            if (autoPrint_)
                std::cout << "clock started: " << clkData_->getClkName() << std::endl;
            if (resetTick)
                clkData_->resetTick();
        }
        void setCurrentIndex(int index) {
            clock_t clk_old = clk_;
            clk_ = clock();
            clkData_->ticks_[currentIndex_] += clk_ - clk_old;
            currentIndex_ = index;
        }
        void print() {
            clock_t clk_old = clk_;
            clk_ = clock();
            clkData_->ticks_[currentIndex_] += clk_ - clk_old;
            clkData_->print();
        }
        ~ClockStart() {
            clock_t clk_old = clk_;
            clk_ = clock();
            clkData_->ticks_[currentIndex_] += clk_ - clk_old;
            if (autoPrint_) {
                clkData_->print();
                std::cout << "clock stopped: " << clkData_->getClkName() << std::endl;
            }
        }
    };
    
    ///////////////////////////////////////////////////////////
    // usage (example)
    ///////////////////////////////////////////////////////////
    // Y::ClockSimple *clk = new Y::ClockSimple("example");
    // (some procedures);
    // delete clk;
    ///////////////////////////////////////////////////////////
    class ClockSimple
    {
        std::string message_;
        clock_t clk_;
    public:
        ClockSimple(const std::string& message = "")
        : message_(message)
        , clk_(clock())
        {
            std::cout << "Start " << message_ << "...";
        }
        ~ClockSimple()
        {
            std::cout << "done!" << "(" << (1000 * (clock() - clk_) / CLOCKS_PER_SEC) << "msec)\n";
        }
    };
    
    class ClockLoop {
        int tick_;
        int tickMax_;
        int printIntervalMilliSec_;
        std::string name_;
        clock_t clk_, clk_init_;
    public:
        ClockLoop()
        : tick_(0)
        , tickMax_(0)
        , printIntervalMilliSec_(0)
        , clk_(0)
        , clk_init_(0)
        {}
        ClockLoop(int tickMax, std::string name = "", int printIntervalMilliSec = 1000) {
            init(tickMax, name, printIntervalMilliSec);
        }
        void init(int tickMax, std::string name = "", int printIntervalMilliSec = 1000) {
            tick_ = 0;
            tickMax_ = tickMax;
            printIntervalMilliSec_ = printIntervalMilliSec;
            name_ = name;
            clk_ = clk_init_ = clock();
        }
        void tick() {
            ++tick_;
            clock_t clk_new = clock();
            if (tick_ == tickMax_ || printIntervalMilliSec_ < (clk_new - clk_) * 1000 / CLOCKS_PER_SEC) {
                double percent = (100. * tick_) / tickMax_;
                int step = static_cast<int>(percent / 10);
                std::cout << name_ << " |";
                for (int i = 0; i < 10; ++i)
                    std::cout << (i < step ? "#" : "-");
                std::cout << "| " << percent << "% ";
                if (tick_ < tickMax_)
                    std::cout << "     \r";
                else
                    std::cout << ((clk_new - clk_init_) / CLOCKS_PER_SEC) <<"sec\n";
                clk_ = clk_new;
            }
        }
        void done() {     // i'm done!
            if (tick_ == tickMax_)
                return;
            tick_ = tickMax_ - 1;
            tick();
        }
    };
    
}

