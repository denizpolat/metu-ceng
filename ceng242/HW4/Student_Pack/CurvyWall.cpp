#include "CurvyWall.h"


/*********************************************************************
*									Constructor
 *
* The first @param is the length of the CurvyWall
* The second @param is the radius of the CurvyWall
 */
CurvyWall::CurvyWall(float len, float rad) : Wall(len) {
    this->radius = rad;
    this->center = NULL;
    this->isLinear = false;
    this->length = len;
}
/*********************************************************************
*									Constructor
*
* The first @param is the start point of CurvyWall
* The second @param is the end point of CurvyWall
* The third @param is the center point of CurvyWall
* The CurvyWall object is defined in counterclockwise direction.
 */
CurvyWall::CurvyWall(const Particle & start, const Particle & end, const Particle & center) : Wall(start, end) {
    this->center = new Particle(center);
    this->radius = start.FindDistance(center);
    this->isLinear = false;
    ComputeLength();
}

/*********************************************************************
*									Copy Constructor
 *
* Deep copy
 */
CurvyWall::CurvyWall(const CurvyWall & rhs) : Wall(rhs) {
    this->center = new Particle(rhs.GetCenter());
    this->radius = rhs.radius;
    this->isLinear = rhs.isLinear;
}
/*********************************************************************
 *									GetRadius
*
* @return the radius of CurvyWall
 */
float CurvyWall::GetRadius() const {
    return this->radius;
}
/*********************************************************************
 *									GetCenter
*
* @return the center point of CurvyWall
 */
const Particle& CurvyWall::GetCenter() const {
    return *center;
}
/*********************************************************************
*									ComputeLength
*
* Computes the length of the CurvyWall object
* Use PI defined in the macros in case that you need
*/
void CurvyWall::ComputeLength() {
    Wall one = Wall(*center, *initial);
    Wall two = Wall(*center, *final);
    float angle = one.FindAngleBetween(two);
    this->length = abs(radius * angle);
}
/*********************************************************************
*									Clone
*
* @return pointer to a copy of this Wall object
*/
Wall* CurvyWall::Clone() const {
    Wall* temp = new CurvyWall(*this);
    return temp;
}
/*********************************************************************
*									IsContinuousLinear
*
 * @return false always
 */
bool CurvyWall::IsContinuousLinear(const Wall&) const {
    return false;
}
/*********************************************************************
*									Operator overload
*
* Combines this CurvyWall object with the Wall given in the @param
* @return the combined wall
*	- The combination can be done only if the wall given in the argument
    is also a CurvyWall such that one wall starts at the point where
      the other ends and their center is common (coordinates of their
      center points are the same)
    - On the other hand, the fact that which one of the two walls starts
      first is not important. Similar to the fact that 3+5 = 5+3.
* For the cases which do not satisfy above, the method should throw
  ApplePearException which was defined in Exception.h.
* The walls to be combined will always be the ones constructed with
  their start and end points (anomalous examples will not be tested.)
 */
const Wall& CurvyWall::operator+(const Wall& rhs) const {
    ApplePearException e;
    if (rhs.isLinear) throw e;

    if(*center == *(rhs.center)) {
        if(*initial == rhs.GetFinalParticle()) {
            CurvyWall* temp = new CurvyWall(rhs.GetInitialParticle(), *final, *center);
            temp->ComputeLength();
            temp->radius = this->radius;
            return *temp;
        }
        else if(*final == rhs.GetInitialParticle()) {
            CurvyWall* temp = new CurvyWall(*initial, rhs.GetFinalParticle(), *center);
            temp->ComputeLength();
            temp->radius = this->radius;
            return *temp;
        }
    }
    else throw e;


}
/*********************************************************************
*									Destructor
*
 */
CurvyWall::~CurvyWall() {
}