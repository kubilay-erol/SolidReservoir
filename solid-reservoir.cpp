#include <cstdint>
#include <new>
#include <cstring>
#include <fixed.hpp>


enum class ReservoirType {
infinite = 1,
finite = 0,
};


fixed::fixed(ReservoirType restyp) { //add conversion constructor 
 
    *this = fixed(static_cast<int32_t>(restyp));   
}



class SolidReservoir {
    
public:
    static SolidReservoir* create_reservoir(SolidReservoir* add, const char* name, fixed temp, int32_t size, fixed c, fixed mass = static_cast<fixed>(ReservoirType::infinite)) {
        if (add == nullptr) {
            return nullptr;             
        }
        if (mass <= 0) {
            return nullptr;
        }
        if (temp <= 0) {
            return nullptr;
        }
        if ((fixed(size) != static_cast<fixed>(ReservoirType::finite)) && (fixed(size) != static_cast<fixed>(ReservoirType::infinite))) {
            return nullptr;
        }
        if (c <= 0) {
            return nullptr;
        }
        
        SolidReservoir* ptr = new (add) SolidReservoir(name, temp, size, c, mass);
        
        return ptr; //return reservoir pointer
    }
    
    bool is_infinite() const {
        if (mass_ == -1) {
            return true;
        }
        
        else {
            return false;
        }
    }
    
    bool is_finite() const {
        if (mass_ != -1) {
            return true;
        }
        
        else {
            return false;
        }
    }
    

    void absorb_heat_from(SolidReservoir& resvr, fixed amount) {
        if (this == &resvr) {
            return;
        }
        
        resvr.heat_transfer(-amount);
        heat_transfer(amount);
    }
    
    
    void release_heat_to(SolidReservoir& resvr, fixed amount) {
        if (this == &resvr) {
            return;
        }
        
        resvr.heat_transfer(amount);
        heat_transfer(-amount);
    }
    

    
    const char* name() const {
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
    fixed thermal_capacity() const {
        if (size_ == static_cast<fixed>(ReservoirType::infinite)) {
            return -1; 
        }
        return mass_ * c_;
    }

    fixed available_heat(fixed ambi_temp) const { //input ambient temperature
        if (fixed(size_) == static_cast<fixed>(ReservoirType::infinite)) {
            return -1; 
        }
        return mass_ * c_ * (temp_ - ambi_temp); //ambient temp is assumed to be inf reservoir
    } //returns highest possible heat that can be rejected by the reservoir
    
    void heat_transfer(fixed heat) {
        if (is_infinite()) {
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

    static void destroy_reservoir(SolidReservoir* res) {
        if (res != nullptr) {
            res->~SolidReservoir();
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
        if (size_ == static_cast<fixed>(ReservoirType::infinite)) {
            mass_ = -1;
            c_ = -1;
        }
    } 

    void set_temp(fixed temp) {
        temp_ = temp;
    } 
    
};
