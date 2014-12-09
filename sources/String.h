//
//  String.h
//  test
//
//  Created by deng on 14-10-13.
//  Copyright (c) 2014年 deng. All rights reserved.
//

#ifndef test_String_h
#define test_String_h

#include <utility>
namespace TinySTL {
    class String {
        friend void swap(String &, String &);
        friend bool operator==(const String &, const String &);
        friend bool operator!=(const String &, const String &);
        friend bool operator<=(const String &, const String &);
        friend bool operator< (const String &, const String &);
        friend bool operator>=(const String &, const String &);
        friend bool operator> (const String &, const String &);
        char * data_;
        unsigned int * ref_cnt_;
        size_t len_;
        
    public:
        
        String() : data_(new char[1]) {
            *data_ = '\0';
            len_ = 0;
            ref_cnt_ = new unsigned int(1);
        }
        
        String(const char * str) : data_(new char[strlen(str) + 1]) {
            strcpy(data_, str);
            len_ = strlen(str);
            ref_cnt_ = new unsigned int(1);
        }
        
        String(const String &rhs) :
        data_(rhs.data_), ref_cnt_(rhs.ref_cnt_), len_(rhs.len_) {
            (*ref_cnt_)++;
        }
        
        ~String() {
            (*ref_cnt_)--;
            if (*ref_cnt_ == 0) {
                try {
                    delete [] data_;
                } catch (std::runtime_error) {
                    std::cerr << "Error happend when deleting memory for String." << std::endl;
                }
            }
        }
        
        String & operator=(const String &rhs) {
            String tmp(rhs);
            swap(*this, tmp);
            return *this;
        }
        
        String(String && rhs) :
        data_(rhs.data_), len_(rhs.len_), ref_cnt_(rhs.ref_cnt_) {
            rhs.data_       = new char[1];
            *rhs.data_      = '\0';
            rhs.len_        = 0;
            rhs.ref_cnt_    = new unsigned int(1);
        }
        
        String & operator=(String && rhs) {
            if (this != &rhs) {
                String tmp(rhs);
                swap(*this, tmp);
                rhs.data_ = new char[1];
                *rhs.data_ = '\0';
                rhs.len_ = 0;
                rhs.ref_cnt_ = new unsigned int(1);
            }
            return *this;
        }
        
        // copy on write
        char & operator[](size_t n) {
            if (n > len_ || data_ == 0) {
                static char nullchar = 0;
                return nullchar;
            }
            // if ref_cnt_ = 1, no need to copy
            if (*ref_cnt_ > 1) {
                (*ref_cnt_)--;
                auto tmp = new char[len_+1];
                strncpy(tmp, data_, len_);
                data_ = tmp;
                ref_cnt_ = new unsigned int(1);
            }
            return data_[n];
        }
        // const version, no need to copy
        const char & operator[](size_t n) const {
            if (n > len_ || data_ == 0) {
                static char nullchar = 0;
                return nullchar;
            }
            return data_[n];
        }
        
        // Accessors
        size_t size() const {
            return len_;
        }
        const char * c_str() const {
            return data_;
        }
        
        
    };
    
    void swap(String &lhs, String &rhs){
        std::swap(lhs.data_, rhs.data_);
        std::swap(lhs.len_, rhs.len_);
        std::swap(lhs.ref_cnt_, rhs.ref_cnt_);
    }
    
    std::ostream & operator<<(std::ostream & os, const String &str)
    {
        os << str.c_str();
        return os;
    }
    
    bool operator==(const String &lhs, const String &rhs) {
        if(strcmp(lhs.data_, rhs.data_) == 0) return true;
        return false;
    }
    
    bool operator!=(const String &lhs, const String &rhs) {
        return !(lhs==rhs);
    }
    
    bool operator<=(const String & lhs, const String &rhs) {
        if(strcmp(lhs.data_, rhs.data_) <= 0) return true;
        return false;
    }
    
    bool operator< (const String & lhs, const String &rhs) {
        if (strcmp(lhs.data_, rhs.data_) < 0) return true;
        return false;
    }
    
    bool operator>=(const String & lhs, const String &rhs) {
        if (strcmp(lhs.data_, rhs.data_) >= 0) return true;
        return false;
    }
    
    bool operator> (const String & lhs, const String &rhs) {
        if (strcmp(lhs.data_, rhs.data_) > 0) return true;
        return false;
    }
}

#endif
