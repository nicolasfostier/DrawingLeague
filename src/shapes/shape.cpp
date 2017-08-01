#include "include/shapes/shape.h"



// Setter
void AbstractShape::setPen(QPen pen){
	this->pen = pen;
}


// Constructor
AbstractShape::AbstractShape(QPen pen){
	this->pen = pen;
}



// Destructor
AbstractShape::~AbstractShape(){

}
