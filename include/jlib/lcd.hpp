
#include <string>
#include <jlib/hal.h>

class LCD{
public:
	LCD* init(I2C_HandleTypeDef* i2c);
	void print(const std::string& text);
	void print2(const std::string& text);

private:
	std::string text1;
	std::string text2;
};
