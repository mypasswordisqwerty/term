#ifndef OBJECT_H
#define OBJECT_H

class Object{
public:
	virtual ~Object(){}
	virtual void loop(uint32_t delay){}
};


#endif
