#ifndef UPDATABLE_H_
#define UPDATABLE_H_

class Updatable {
public:
	virtual ~Updatable() {};
	virtual void step(float frame, float time) = 0;
};

#endif /* UPDATABLE_H_ */
