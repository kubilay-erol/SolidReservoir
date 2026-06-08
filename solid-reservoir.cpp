#include <cstdint>
#include <new>
#include <cstring>
#include "fixed.h"

class SolidReservoir {
    
public:
    static SolidReservoir* create_reservoir(SolidReservoir* add, const char* name, fixed temp, int32_t size, fixed c, fixed mass = 1) {
        if (add == nullptr) {
            return nullptr;
        }
        if (mass <= 0) {
            return nullptr;
        }
        if (temp <= 0) {
            return nullptr;
        }
        if ((size != 0) && (size != 1)) {
            return nullptr;
        }
        if (c <= 0) {
            return nullptr;
        }
        
        SolidReservoir* ptr = new (add) SolidReservoir(name, temp, size, c, mass);
        
        return ptr; //return reservoir pointer
    }
    
    const char* name() {
        return name_;
    }
    
    fixed temp() const {
        return temp_;
    }
    
    int32_t size() const {
        return size_;
    }
    
    fixed mass() const {
        return mass_;
    }
    
    fixed c() const {
        return c_;
    }
    
    void heat_transfer(fixed heat) {
        if (mass_ == -1) {
            return;
        }
        
        else {
            temp_ = (heat/(mass_*c_)) + temp_;
        }
    }
    
    void equilibrium(SolidReservoir* other) {
        fixed fin_temp = 0;

        if ((other->mass_ != -1) && (this->mass_ != -1)) {  //both non-inf reservoir 
            fin_temp = ((other->mass_)*(other->temp_)*(other->c_) + (this->mass_)*(this->temp_)*(this->c_))/((other->mass_)*(other->c_)+(this->mass_)*(this->c_));
            this->set_temp(fin_temp);
            other->set_temp(fin_temp);
        }
        
        else if ((other->mass_ == -1) && (this->mass_== -1)) { //both infinite reservoir
            return;
        }

        else if ((other->mass_ == -1) && (this->mass_ != -1)) { //infinite-finite reservoir
            this->set_temp(other->temp());
        }

        else if ((other->mass_ != -1) && (this->mass_ == -1)) { //finite-infinite reservoir 
            other->set_temp(this->temp());
        }        
    }
    
private: 

    char name_[32] = {}; 
    fixed temp_ = 0;
    int32_t size_ = 0; //1-infinite reservoir, 0-finite reservoir
    fixed mass_ = 0;
    fixed c_ = 0; // specific heat capacity of the reservoir

    SolidReservoir(const char* n, fixed t, int32_t s, fixed c, fixed m) : temp_(t), size_(s), c_(c), mass_(m) {
        strncpy(name_, n, 31);
        name_[31] = '\0';
        if (size_ == 1) {
            mass_ = -1;
            c_ = -1;
        }
    } 

    void set_temp(fixed temp) {
        temp_ = temp;
    } 
    
};
