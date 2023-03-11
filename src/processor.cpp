#include "processor.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
    float cpu_util = 0.0;
    std::vector<int> curr_values = GetCPUInfo();
    if (flag) cpu_util = CalculatePercentage(curr_values, prev_values);
    this->prev_values = curr_values;
    flag = true;
    return cpu_util;
}

std::vector<int> Processor::GetCPUInfo(){
    std::ifstream input_file(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    std::vector<int> values;
    std::string line, key;
    int tmp;
    if (input_file.is_open()){
        while(std::getline(input_file, line)){
            std::istringstream linestream(line);
            linestream >> key;
            if (key == "cpu"){
                while(linestream >> tmp){
                    values.push_back(int(tmp));
                }
                return values;
            }
        } 
    }
    return std::vector<int>();
}

float Processor::CalculatePercentage(const std::vector<int>& curr_v, const std::vector<int>& prev_v){
    
    // user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
    // PrevNonIdle = user + nice + system + irq + softrig + steal
    // PrevIdle = idle + iowait
    // PrevTotal = Idle + NonIdle

    // CurrNonIdle = user + nice + system + irq + softrig + steal
    // CurrIdle = idle + iowait
    // CurrTotal = Idle + NonIdle

    // CPU_Percentage=((CurrTotal - PrevTotal) - (CurrIdle - PrevIdle))/(CurrTotal - PrevTotal)*100
    int PrevNonIdle = prev_v[user] 
                         + prev_v[nice] 
                         + prev_v[system] 
                         + prev_v[irq] 
                         + prev_v[softirq] 
                         + prev_v[steal];
    int PrevIdle = prev_v[idle] + prev_v[iowait];
    int PrevTotal = PrevIdle + PrevNonIdle;

    int CurrNonIdle = curr_v[user] 
                         + curr_v[nice] 
                         + curr_v[system] 
                         + curr_v[irq] 
                         + curr_v[softirq] 
                         + curr_v[steal];
    int CurrIdle = curr_v[idle] + curr_v[iowait];
    int CurrTotal = CurrIdle + CurrNonIdle;
    return (((CurrTotal - PrevTotal) - (CurrIdle - PrevIdle))/float(CurrTotal - PrevTotal));
}