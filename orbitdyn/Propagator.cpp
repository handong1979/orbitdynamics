#include <BaseException.h>
#include <Propagator.h>
#include <math.h>

//------------------------------------------------------------------------------
// Propagator::Propagator(void)
//------------------------------------------------------------------------------
/**
 * Default base class constructor
 * The default constructor for the Propagator class sets the stepSize to 60 
 * seconds, sets the member pointers to NULL, the dimension to 0, and the 
 * initialized flag to false.
 */
//------------------------------------------------------------------------------
Propagator::Propagator(void)
	:stepSize           (60.0),
	 stepSizeBuffer      (60.0),
	 initialized         (false),
	 resetInitialData    (true),
	 inState             (NULL),
	 outState            (NULL),
	 dimension           (0),
	 physicalModel       (NULL),
	 finalStep           (false)
{

}

//------------------------------------------------------------------------------
// Propagator::~Propagator()
//------------------------------------------------------------------------------
/**
 * Base class destructor
 * The base class destructor does not perform any actions
 */
//------------------------------------------------------------------------------
Propagator::~Propagator(void)
{
}


//------------------------------------------------------------------------------
// Propagator::Propagator(const Propagator& p)
//------------------------------------------------------------------------------
/**
 * The copy constructor
 */
//------------------------------------------------------------------------------
Propagator::Propagator(const Propagator& p)
    : stepSize            (p.stepSize),
      stepSizeBuffer      (p.stepSizeBuffer),
      initialized         (false),
      resetInitialData    (true),
      inState             (NULL),
      outState            (NULL),
	  dimension           (p.dimension),
	  physicalModel       (NULL),
      finalStep           (false)
{    
}

//------------------------------------------------------------------------------
// Propagator& Propagator::operator=(const Propagator& p)
//------------------------------------------------------------------------------
/**
 * Assignment operator
 */
//------------------------------------------------------------------------------
Propagator& Propagator::operator=(const Propagator& p)
{
    if (this == &p)
        return *this;

    stepSize       = p.stepSize;
    stepSizeBuffer = p.stepSizeBuffer;
    dimension      = p.dimension;

    inState = outState = NULL;

    initialized = false;
    resetInitialData = true;
    finalStep = false;    

    return *this;
}

//------------------------------------------------------------------------------
// bool Propagator::Initialize()
//------------------------------------------------------------------------------
/**
 * Initialize the propagation system
 * This method is used to perform any internal initialization needed by the 
 * propagator, and to initialize the corresponding PhysicalModel.
 * This implementation sets the inState and outState pointers to point to the
 * PhysicalModel's state vector.  You can change that choice by overriding this
 * method in your derived classes.
 */
//------------------------------------------------------------------------------
bool Propagator::Initialize()
{
   if (physicalModel != NULL) 
   {
       //if ( physicalModel->Initialize() )
       //   initialized = true;

       //inState  = physicalModel->GetState();
       //outState = physicalModel->GetState();
       
       if (resetInitialData)
       {
          stepSize = stepSizeBuffer;
          resetInitialData = false;
       }
    }
    else
       throw PropagatorException("Propagator::Initialize -- Force model is not defined");
    
    if (!initialized)
       throw PropagatorException("Propagator failed to initialize");

    return true;
}

//------------------------------------------------------------------------------
// void Propagator::SetPhysicalModel(PhysicalModel *pPhyscialModel)
//------------------------------------------------------------------------------
/**
 * Connects the propagation system to the physical model
 *
 * @param pPhyscialModel    Pointer to the physical model
 */
//------------------------------------------------------------------------------
void Propagator::SetPhysicalModel(PhysicalModel *pPhysicalModel)
{
    //physicalModel = pPhysicalModel;
}

//------------------------------------------------------------------------------
// void Propagator::Update()
//------------------------------------------------------------------------------
/**
 * Envoked to force a propagator reset if the PhysicalModel changes
 */
//------------------------------------------------------------------------------
void Propagator::Update(bool forwards)
{
   if (resetInitialData)
   {
      stepSize = stepSizeBuffer;
      resetInitialData = false;
   }
   else
   {
      stepSize = (forwards ? fabs(stepSize) : -fabs(stepSize));
   }
}

//------------------------------------------------------------------------------
// void Propagator::ResetInitialData()
//------------------------------------------------------------------------------
/**
 * Sets the propagator for the first call in a run.
 */
//------------------------------------------------------------------------------
void Propagator::ResetInitialData()
{
   resetInitialData = true;
}

//------------------------------------------------------------------------------
// const Real * Propagator::AccessOutState()
//------------------------------------------------------------------------------
/**
 * Provide a look at the output state
 */
//------------------------------------------------------------------------------
const double * Propagator::AccessOutState()
{
    return outState;
}

//------------------------------------------------------------------------------
// Integer Propagator::GetPropagatorOrder() const
//------------------------------------------------------------------------------
/**
 * Returns derivative order needed by the propagator; 0 for analytic
 */
//------------------------------------------------------------------------------
int Propagator::GetPropagatorOrder() const
{
    return 0;
}

//------------------------------------------------------------------------------
// bool Propagator::Step(Real dt)
//------------------------------------------------------------------------------
/**
 * Evolves the physical model over the specified time
 * This method sets the default timestep to the input value, and then advances 
 * the system by one timestep.  If the system has not been initialized, then
 * no action is taken.  
 * If the step is taken successfully, the method returns true; otherwise, it 
 * returns false.
 *
 * @param dt    The timestep to take
 */
//------------------------------------------------------------------------------
bool Propagator::Step(double dt)
{
    if (initialized)
    {
        stepSize = dt;
        return Step();
    }
    return false;
}


//------------------------------------------------------------------------------
// void SetAsFinalStep(bool fs)
//------------------------------------------------------------------------------
/**
 * Method used to toggle the finalStep flag.
 * 
 * @param fs The setting for the flag.
 */
//------------------------------------------------------------------------------
void Propagator::SetAsFinalStep(bool fs)
{
  
   finalStep = fs;
}


//------------------------------------------------------------------------------
// bool RawStep(Real dt)
//------------------------------------------------------------------------------
/**
 * Method used to take an uncontrolled step of a specified size
 * This method sets the stepSize to the input value and then calls the class's 
 * RawStep() method.
 * 
 * @param dt    The desired raw step size
 */
//------------------------------------------------------------------------------
bool Propagator::RawStep(double dt)
{
   double ctlstepsize = stepSize;
    stepSize = dt;
    bool retval = RawStep();
    stepSize = ctlstepsize;
    return retval;
}

