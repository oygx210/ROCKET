#pragma ident "$Id$"

/**
 * @file SimpleFilter.hpp
 * This class filters out satellites with observations grossly out of bounds.
 */

#ifndef GPSTK_SIMPLEFILTER_HPP
#define GPSTK_SIMPLEFILTER_HPP

//============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 2.1 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//
//  Dagoberto Salazar - gAGE ( http://www.gage.es ). 2007, 2008, 2011
//
//============================================================================
//
//  Revision
//  ========
//
//  2014/10/15  Throw 'SVNumException' if the satellite number is less than 4.
//
//  Copywright(c) 2014 - ., Shoujian Zhang, Wuhan University
//
//============================================================================



#include "ProcessingClass.hpp"


namespace gpstk
{

      /** @addtogroup DataStructures */
      //@{


      /** This class filters out satellites with observations grossly out of
       *  bounds.
       *
       * This class is meant to be used with the GNSS data structures objects
       * found in "DataStructures" class.
       *
       * A typical way to use this class follows:
       *
       * @code
       *   RinexObsStream rin("ebre0300.02o");
       *
       *   gnssRinex gRin;
       *   SimpleFilter myFilter;
       *
       *   while(rin >> gRin)
       *   {
       *      gRin >> myFilter;
       *   }
       * @endcode
       *
       * Warning:
       * The filter will firstly check the satellite number stored
       * in 'gRin', if the number is too small, e.g. less than 2, the
       * 'SVNumException' will be thrown, indicating data exception for current
       * epoch.
       *
       * The "SimpleFilter" object will visit every satellite in the GNSS data
       * structure that is "gRin" and will check that the given code
       * observations are within some (preassigned) boundaries.
       *
       * By default, the algorithm will check C1 observables, the minimum limit
       * is 15000000.0 meters and the maximum limit is 30000000.0 meters. You
       * may change all these settings with the appropriate set methods.
       *
       * Also, you may set more than one observable to be checked by passing a
       * "TypeIDSet" object to the appropriate constructors or methods. For
       * instance:
       *
       * @code
       *   TypeIDSet typeSet;
       *   typeSet.insert(TypeID::C1);
       *   typeSet.insert(TypeID::C2);
       *
       *   myFilter.setFilteredType(typeSet);
       * @endcode
       *
       * Be warned that if a given satellite does not have the observations
       * required, or if their are out of bounds, the full satellite record
       * will be summarily deleted from the data structure.
       *
       */
   class SimpleFilter : public ProcessingClass
   {
   public:

         /// Default constructor. By default, filter C1.
      SimpleFilter() : minLimit(15000000.0), maxLimit(30000000.0)
      { setFilteredType(TypeID::C1); };


         /** Explicit constructor
          *
          * @param type      TypeID to be filtered.
          * @param min       Minimum limit (in meters).
          * @param max       Maximum limit (in meters).
          */
      SimpleFilter( const TypeID& type,
                    const double& min,
                    const double& max )
         : minLimit(min), maxLimit(max)
      { setFilteredType(type); };


         /** Explicit constructor
          *
          * @param type      TypeID to be filtered.
          */
      SimpleFilter(const TypeID& type)
         : minLimit(15000000.0), maxLimit(30000000.0)
      { setFilteredType(type); };


         /** Explicit constructor
          *
          * @param typeSet   Set of TypeID's to be filtered.
          * @param min       Minimum limit (in meters).
          * @param max       Maximum limit (in meters).
          */
      SimpleFilter( const TypeIDSet& typeSet,
                    const double& min,
                    const double& max )
         : filterTypeSet(typeSet), minLimit(min), maxLimit(max)
      { };


         /** Explicit constructor
          *
          * @param typeSet   Set of TypeID's to be filtered.
          */
      SimpleFilter(const TypeIDSet& typeSet)
         : filterTypeSet(typeSet), minLimit(15000000.0), maxLimit(30000000.0)
      { };


         /** Returns a satTypeValueMap object, filtering the target
          *  observables.
          *
          * @param gData     Data object holding the data.
          */
      virtual satTypeValueMap& Process(satTypeValueMap& gData)
         throw(ProcessingException, SVNumException);


         /** Returns a gnssSatTypeValue object, filtering the target
          *  observables.
          *
          * @param gData    Data object holding the data.
          */
      virtual gnssSatTypeValue& Process(gnssSatTypeValue& gData)
         throw(ProcessingException, SVNumException)
      { Process(gData.body); return gData; };



         /** Returns a gnssRinex object, filtering the target observables.
          *
          * @param gData    Data object holding the data.
          */
      virtual gnssRinex& Process(gnssRinex& gData)
         throw(ProcessingException, SVNumException)
      { Process(gData.body); return gData; };



         /** Returns a gnssDataMap object, filtering the target observables.
          *
          * @param gData    Data object holding the data.
          */
      virtual gnssDataMap& Process(gnssDataMap& gData)
         throw(ProcessingException, SVNumException);



         /** Method to set the minimum limit.
          * @param min       Minimum limit (in meters).
          */
      virtual SimpleFilter& setMinLimit(const double& min)
      { minLimit = min; return (*this); };


         /// Method to get the minimum limit.
      virtual double getMinLimit() const
      { return minLimit; };


         /** Method to set the maximum limit.
          * @param max       Maximum limit (in meters).
          */
      virtual SimpleFilter& setMaxLimit(const double& max)
      { maxLimit = max; return (*this); };


         /// Method to get the maximum limit.
      virtual double getMaxLimit() const
      { return maxLimit; };


         /** Method to add a TypeID to be filtered.
          * @param type      Extra TypeID to be filtered.
          */
      virtual SimpleFilter& addFilteredType(const TypeID& type)
      { filterTypeSet.insert(type); return (*this); };


         /** Method to set a TypeID to be filtered. This method will erase
          *  previous types.
          * @param type      TypeID to be filtered.
          */
      virtual SimpleFilter& setFilteredType(const TypeID& type)
      { filterTypeSet.clear(); filterTypeSet.insert(type); return (*this); };


         /** Method to set the TypeID's to be filtered. This method will erase
          *  previous types.
          * @param typeSet       Set of TypeID's to be filtered.
          */
      virtual SimpleFilter& setFilteredType(const TypeIDSet& typeSet)
      { filterTypeSet.clear(); filterTypeSet = typeSet; return (*this); };


         /// Method to get the set of TypeID's to be filtered.
      virtual TypeIDSet getFilteredType() const
      { return filterTypeSet; };


         /// Returns a string identifying this object.
      virtual std::string getClassName(void) const;


         /// Destructor
      virtual ~SimpleFilter() {};


   protected:


         /** Checks that the value is within the given limits.
          * @param value     The value to be test
          *
          * @return
          *  True if check was OK.
          */
      virtual bool checkValue(const double& value) const
      { return ( (value>=minLimit) && (value<=maxLimit) ); };


         /// Set of types to be filtered
      TypeIDSet filterTypeSet;

         /// Minimum value allowed for input data (in meters).
      double minLimit;

         /// Maximum value allowed for input data (in meters).
      double maxLimit;


   }; // End of class 'SimpleFilter'

      //@}

}  // End of namespace gpstk

#endif   // GPSTK_SIMPLEFILTER_HPP
