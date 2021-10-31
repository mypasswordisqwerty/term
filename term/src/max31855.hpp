#ifndef MAX31855_H
#define MAX31855_H

#include <jlib/spidev.hpp>
#include <vector>
#include "utils.hpp"

#define REQ_TIME 500
#define FADDR 0x0800F800

struct Pair{
	int32_t first;
	int32_t second;
};

class MAX31855: public SPIDev{
public:
	MAX31855():_value(0), err(9), reqtime(0), id(0){}

	MAX31855* init(SPI_HandleTypeDef* spi, const Pin& csPin, int id) {
		SPIDev::init(spi, csPin);
		this->id = id;
		return this;
	}

	std::string value() {
		if (err!=0){
			return "ER" + std::to_string(err);
		}

		std::string s = std::to_string(_value);
		while(s.length()<3) s = '0' + s;
		return s;
	}

	inline int temperature() {return _value;}

	virtual void loop(uint32_t delay) override {
		reqtime -= delay;
		if (!cs.valid() || reqtime > 0){
			return;
		}
		reqtime = REQ_TIME;
		int16_t val[] = {0,0};
		if (read(val, 4)!=HAL_OK){
			err = 9;
			return;
		}
		_value = convert(val);
	}

	std::string setTune(std::string& tune){
		std::vector<Pair> tune1;
		std::vector<Pair> tune2;
		std::string err = readReplaceTunes(tune1, tune2, tune);
		if (!err.empty()){
			return err;
		}
		// save to flash;
		if (HAL_FLASH_Unlock()!=HAL_OK)
			  return "Error: flash unlock " + std::to_string(HAL_FLASH_GetError());

		static FLASH_EraseInitTypeDef EraseInitStruct;
		EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
		EraseInitStruct.PageAddress = FADDR;
		EraseInitStruct.NbPages = 1;
		uint32_t PAGEError;
		if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
			  return "Error: flash erase " + std::to_string(HAL_FLASH_GetError());

		uint32_t *addr = (uint32_t*)FADDR;
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)addr++, tune1.size());
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,(uint32_t)addr++, tune2.size());
		for (size_t i=0; i<tune1.size(); i++){
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,(uint32_t)addr++, tune1[i].first);
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,(uint32_t)addr++, tune1[i].second);
		}
		for (size_t i=0;i<tune2.size(); i++){
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,(uint32_t)addr++, tune2[i].first);
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,(uint32_t)addr++, tune2[i].second);
		}

		HAL_FLASH_Lock();
		return "ok";
	}

	std::string getTune(){
		std::string ret;
		int sz;
		Pair* p = getTunePair(sz);
		if (!p){
			return "Error: No tune table";
		}
		for (int i=0; i<sz; i++, p++){
			if (!ret.empty()){
				ret += " ";
			}
			ret += std::to_string(p->first);
			ret += " ";
			ret += std::to_string(p->second);
		}
		return ret;
	}

private:
	int convert(int16_t* vals) {
		err = vals[0] & 7;
		if (err==0 && vals[1]&1){
			err = 8;
		}
		//float t = (vals[0] >> 4) * 0.0625;
		float t = (vals[1] >> 2) * 0.25;

		// tune
		int cnt = 0;
		Pair *p = getTunePair(cnt);
		if (!p){
			return (int)t;
		}
		int i;
		for (i=0; i<cnt && p->first<t; i++, p++);
		Pair *prev = p-1;
		if (i>cnt){
			t *= prev->second/prev->first;
		}else if (i!=0){
			float v = (t - prev->first) / (p->first - prev->first);
			t = p->second + (p->second - prev->second) * v;
		}

		return (int)t;
	}

	Pair* getTunePair(int& count){
		Pair *p = (Pair*)FADDR;
		count = 0;
		if (p->first == -1) return NULL;
		count = id==1 ? p->first : p->second;
		p += id == 1 ? 1 : p->first + 1;
		return p;
	}

	std::string readReplaceTunes(vector<Pair>& tune1, vector<Pair>& tune2, std::string tune){
		std::vector<int32_t> values;
		while(!tune.empty()){
			std::string s = Utils::shift(tune);
			int32_t v = std::stoi(s);
			if (v<=0){
				return "Error: Bad tune value: "+s;
			}
			values.push_back(v);
		}
		if (values.size()%2!=0){
			return "Error: Not even tune count";
		}
		Pair *p = (Pair*)FADDR;
		Pair sz = *p;
		p++;
		for (int i=0; i<sz.first; i++){
			tune1.push_back(*p++);
		}
		for (int i=0; i<sz.second; i++){
			tune2.push_back(*p++);
		}
		std::vector<Pair>* trg = id == 1 ? &tune1 : &tune2;
		trg->clear();
		for (size_t i=0; i<values.size()/2; i++){
			Pair data{values[i*2], values[i*2+1]};
			trg->push_back(data);
		}
		return "";
	}

private:
	int _value;
	int err;
	int reqtime;
	int id;

};


#endif
