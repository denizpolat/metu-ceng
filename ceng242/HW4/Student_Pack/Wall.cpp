#include "Wall.h"

/*********************************************************************
    *									Constructor
    *
    * @param is the length of the Wall
    */
Wall::Wall(float len) {
    this->length = len;
    this->initial = NULL;
    this->final = NULL;
    this->isLinear = true;
}
/*********************************************************************
*									Constructor
*
* The first @param is the start point of Wall
* The second @param is the end point of Wall
* The Wall object is defined in counterclockwise direction.
*/
Wall::Wall(const Particle& start, const Particle& end) {
    this->initial = new Particle(start);
    this->final = new Particle(end);
    this->isLinear = true;
    ComputeLength();
}
/*********************************************************************
*									Copy constructor
*
* Deep copy
*/
Wall::Wall(const Wall& rhs) {
    this->initial = new Particle(rhs.GetInitialParticle());
    this->final = new Particle(rhs.GetFinalParticle());
    this->length = rhs.length;
    this->isLinear = rhs.isLinear;
}
/*********************************************************************
*									GetInitialParticle
*
* @return the start point of Wall
* This method will be called only for the walls constructed with their start
  and end points
*/
const Particle& Wall::GetInitialParticle() const {
    return *initial;
}
/*********************************************************************
*									GetFinalParticle
*
* @return the end point of Wall
* This method will be called only for the walls constructed with their start
  and end points
*/
const Particle& Wall::GetFinalParticle() const {
    return *final;
}
/*********************************************************************
*									FindAngleBetween
*
* @return the angle between this Wall object and the one given in the @param
*/


float Wall::FindAngleBetween(const Wall& rhs) const {  // rhs final - rhs init
    Particle p1 = Particle((final->X() - initial->X()), (final->Y() - initial->Y()));
    Particle p2 = Particle((rhs.GetFinalParticle().X() - rhs.GetInitialParticle().X()), (rhs.GetFinalParticle().Y() - rhs.GetInitialParticle().Y()));
    float thetathis = atan2(p1.Y(), p1.X());
    float thetarhs = atan2(p2.Y(), p2.X());
    if(thetathis < 0) thetathis += 2 * M_PI;
    if(thetarhs < 0) thetarhs += 2 * M_PI;
    float tmp = thetarhs - thetathis;
    while(tmp < 0) tmp += 2 * M_PI;
    return (tmp == 0) ? 2*M_PI : tmp;
}
/*********************************************************************
*
 *						GetLength
*
* @return the length of Wall
*/
float Wall::GetLength() const {
    return this->length;
}
/*********************************************************************
*									ComputeLength
*
* Computes the length of the Wall object
*/
void Wall::ComputeLength() {
    this->length = initial->FindDistance(this->GetFinalParticle());
}
/*********************************************************************
*									Clone
*
* @return pointer to a copy of this Wall object
*/
Wall* Wall::Clone() const {
    Wall* temp = new Wall(*this);
    return temp;
}
/*********************************************************************
*									IsContinuousLinear
*
* Checks whether the end points of this Wall object and the end points
  of the Wall given in the @param are linear and the walls are continuous
* @return true if they are linear and continuous, false otherwise
* The method returns always false in case that the wall given in the
  argument is a curvy wall
* Two walls are linear only if they are straight walls and their slopes
  are equal (Be cautious while calculating slope of the vertical lines!)
* Two walls are continuous only if one starts at the point where the
  other ends (the fact that which one of the two walls starts first is
  not important.)
* The walls to be combined will always be the ones constructed with
  their start and end points (anomalous examples will not be tested.)
*/
bool Wall::IsContinuousLinear(const Wall& rhs) const {
    float angle = this->FindAngleBetween(rhs);
    bool result = (((fabs(angle - 2 * M_PI) <= EPSILON) || angle <= EPSILON) && ((*initial == rhs.GetFinalParticle()) || (*final == rhs.GetInitialParticle())));
    return result;
}
/*********************************************************************
*									Operator overload
*
* Combines this Wall object with the Wall given in the @param
* @return the combined wall
* Two walls can be combined only if one of the two cases below are
  satisfied:
    - They are both straight walls such that one starts at the point
      where the other ends
    - They are both curvy walls such that one starts at the point where
    the other ends and their center is common (coordinates of their
      center points are the same)
     - On the other hand, the fact that which one of the two walls starts
      first is not important. Similar to the fact that 3+5 = 5+3.
* For the cases which do not satisfy above, the method should throw
  ApplePearException which was defined in Exception.h.
* The walls to be combined will always be the ones constructed with
  their start and end points (anomalous examples will not be tested.)
*/
const Wall& Wall::operator+(const Wall& rhs) const {
    ApplePearException e;
    if(this->IsContinuousLinear(rhs)) {
        if (*(this->initial) == rhs.GetFinalParticle()) {
            Wall *curr = new Wall(rhs.GetInitialParticle(), *(this->final));
            curr->length = this->length + rhs.length;
            return *curr;
        } else if (*(this->final) == rhs.GetInitialParticle()) {
            Wall *curr = new Wall(*(this->initial), rhs.GetFinalParticle());
            curr->length = this->length + rhs.length;
            return *curr;
        }
    }
    else throw e;
}
/*********************************************************************
*									Destructor
*
*/
Wall::~Wall() {
    delete initial;
    delete final;
    delete center;
}
/********************************************************************/