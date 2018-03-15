#include <Config.h>
#pragma once

class PhysicalModel;

class ORBITDYN_API Propagator
{
public:
   Propagator();
   virtual ~Propagator(void);
   
   Propagator(const Propagator&);
   Propagator& operator=(const Propagator&);
   
   
   virtual bool Initialize();
   virtual void SetPhysicalModel(PhysicalModel *pPhysicalModel);
   virtual bool Step(double dt);
   virtual void SetAsFinalStep(bool fs);

   virtual void Update(bool forwards = true);
   virtual void ResetInitialData();
   const double* AccessOutState();
   
   virtual int GetPropagatorOrder(void) const;    
   
   //------------------------------------------------------------------------------
   // virtual bool Step(void)
   //------------------------------------------------------------------------------
   /**
    * Method to propagate by the default step
    * This pure virtual method provides the interface used to advance the state
    * by the amount specified in the stepSize parameter.  This is the core 
    * propagation routine.  
    * Adaptive step propagators will take the desired step if accuracy permits, 
    * and then adjust the stepSize parameter to the estimated optimal size for the 
    * next call.
    */
   //------------------------------------------------------------------------------
   virtual bool Step() = 0;
   
   //------------------------------------------------------------------------------
   // virtual bool RawStep(void)
   //------------------------------------------------------------------------------
   /**
    * Method to take a raw propagation step without error control
    */
   virtual bool RawStep() = 0;
   
   //------------------------------------------------------------------------------
   // virtual bool GetStepTaken()
   //------------------------------------------------------------------------------
   /**
    * Method to report the size of the most recent propagation step.
    */
   virtual double GetStepTaken() = 0;
   
   virtual bool RawStep(double dt);
   
protected:
    
   /// Size of the default time step
   double stepSize;
   /// Buffer for the stepsize.
   double stepSizeBuffer;
   /// Flag used to determine if the propagator has been initialized
   bool initialized;
   /// Flag used to determine if the propagator need to reset the initial step
   bool resetInitialData;
   /// Pointer to the input state vector
   double *inState;
   /// Pointer to the location for the propagated state
   double *outState;
   /// Number of parameters being propagated
   int dimension;
   /// Pointer to the information related to the physics of the system
   PhysicalModel *physicalModel;
   /// Flag used to detect if the code is taking the last Propagate step
   bool finalStep; 
};

class PropagatorException : public BaseException
{
public:
	PropagatorException(const std::string& message = "Unspecified") 
		: BaseException("Propagator Exception: ", message) {};
};