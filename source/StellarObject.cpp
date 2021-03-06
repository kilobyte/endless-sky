/* StellarObject.cpp
Copyright (c) 2014 by Michael Zahniser

Endless Sky is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

Endless Sky is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "StellarObject.h"

#include "Color.h"
#include "GameData.h"
#include "Planet.h"
#include "Politics.h"

#include <algorithm>

using namespace std;



// Object default constructor.
StellarObject::StellarObject()
	: planet(nullptr),
	distance(0.), speed(0.), offset(0.), parent(-1),
	message(nullptr), isStar(false), isStation(false), isMoon(false)
{
}



// Some objects do not have sprites, because they are just an orbital
// center for two or more other objects.
const Animation &StellarObject::GetSprite() const
{
	return animation;
}



// Get this object's position on the date most recently passed to this
// system's SetDate() function.
const Point &StellarObject::Position() const
{
	return position;
}



// Get the unit vector representing the rotation of this object.
const Point &StellarObject::Unit() const
{
	return unit;
}



// Get the radius of this planet, i.e. how close you must be to land.
double StellarObject::Radius() const
{
	double radius = -1.;
	if(!animation.IsEmpty())
		radius = .5 * min(animation.Width(), animation.Height());
	
	// Special case: stars may have a huge cloud around them, but only count the
	// core of the cloud as part of the radius.
	if(isStar)
		radius = min(radius, 80.);
	
	return radius;
}



// If it is possible to land on this planet, this returns the Planet
// objects that gives more information about it. Otherwise, this
// function will just return nullptr.
const Planet *StellarObject::GetPlanet() const
{
	return planet;
}



// Only planets that you can land on have names.
const string &StellarObject::Name() const
{
	static const string UNKNOWN = "???";
	return (planet && !planet->Name().empty()) ? planet->Name() : UNKNOWN;
}



// If it is impossible to land on this planet, get the message
// explaining why (e.g. too hot, too cold, etc.).
const string &StellarObject::LandingMessage() const
{
	if(!planet && Radius() >= 130.)
	{
		static const string GAS = "You cannot land on a gas giant.";
		return GAS;
	}
	static const string EMPTY;
	return (message) ? *message : EMPTY;
}



// Get the color to be used for displaying this object.
const Color &StellarObject::TargetColor() const
{
	static const Color planetColor[6] = {
		Color(1., 1., 1., 1.),
		Color(.3, .3, .3, 1.),
		Color(0., .8, 1., 1.),
		Color(.8, .4, .2, 1.),
		Color(.8, .3, 1., 1.),
		Color(0., .8, 0., 1.)
	};
	int index = !IsStar() + (GetPlanet() != nullptr);
	if(GetPlanet())
	{
		if(!GetPlanet()->CanLand())
			index = 3;
		if(GetPlanet()->IsWormhole())
			index = 4;
		if(GameData::GetPolitics().HasDominated(GetPlanet()))
			index = 5;
	}
	return planetColor[index];
}



// Check if this is a star.
bool StellarObject::IsStar() const
{
	return isStar;
}



// Check if this is a station.
bool StellarObject::IsStation() const
{
	return isStation;
}



// Check if this is a moon.
bool StellarObject::IsMoon() const
{
	return isMoon;
}



// Get this object's parent index (in the System's vector of objects).
int StellarObject::Parent() const
{
	return parent;
}



// Find out how far this object is from its parent.
double StellarObject::Distance() const
{
	return distance;
}
