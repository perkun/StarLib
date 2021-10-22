#ifndef STRATEGY_H_
#define STRATEGY_H_

#include <iostream>
#include <stdio.h>
#include <vector>
#include <memory>
#include <functional>
#include "Potential.h"
#include "starlibmath.h"

namespace StarLib
{

class Simulation;



class Strategy
{
public:
	Strategy() {}
    Strategy(Simulation *simulation);
    ~Strategy() { }

	void set_context(Simulation *simulation);
	Simulation* get_context() { return context; }

protected:
	Simulation* context = nullptr;

};


class ForceStrategy : public Strategy
{
public:
    void execute(std::vector<Vec3> &pos, std::vector<Vec3> &vel,
                         double t, std::vector<Vec3> &g);
	ForceStrategy* push_function(std::function<void(std::vector<Vec3> &pos,
					   std::vector<Vec3> &vel, double t, std::vector<Vec3> &g)>
		fn);


protected:
    std::vector<
        std::function<
						void(std::vector<Vec3> &pos, std::vector<Vec3> &vel,
								double t, std::vector<Vec3> &g)
					>
			   > functions;

	// functions that can be attached to specific strategy
	void dummy(std::vector<Vec3> &pos, std::vector<Vec3> &vel, double t, std::vector<Vec3> &g);

};


class StepStrategy : public Strategy
{
public:
    void execute(std::vector<Vec3> &pos, std::vector<Vec3> &vel,
                         double t);
	StepStrategy* push_lambda(std::function<void(std::vector<Vec3> &pos,
					   std::vector<Vec3> &vel, double t)>
		fn)
	{
		functions.push_back(fn);
		return this;
	}


	template <typename Func>
	StepStrategy* push_member_func(Func fn)
	{
		auto binded_fn = std::bind(fn, this, std::placeholders::_1,
								   std::placeholders::_2,
								   std::placeholders::_3);

		functions.push_back(binded_fn);
		return this;
	}


protected:
    std::vector<
        std::function<
						void(std::vector<Vec3> &pos, std::vector<Vec3> &vel,
								double t)
					>
			   > functions;

public:
	void print_mass(std::vector<Vec3> &pos, std::vector<Vec3> &vel, double t);

};


class StopStrategy: public Strategy
{
public:
	StopStrategy() {}
	StopStrategy(Simulation *simulation);
	~StopStrategy() {}

    virtual bool should_stop(std::vector<Vec3> &pos, std::vector<Vec3> &vel,
                             double t);
protected:
};



} // namespace StarLib

#endif /* STRATEGY_H_ */
