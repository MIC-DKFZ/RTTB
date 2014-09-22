// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the above copyright notices for more information.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
*/ 
#ifndef __FIELD_H
#define __FIELD_H

#include <assert.h>
#include <cstring>

#include "rttbBaseType.h"
#include "rttbMaskType_LEGACY.h"
#include "rttbBaseType_LEGACY.h"
#include "rttbIndexOutOfBoundsException.h"

namespace rttb{
	namespace masks{
		namespace legacy{
			/*! @class BaseField
			@brief This is a class representing a field to be inherited.
			*/
			class BaseField
			{
			public:
				BaseField(){}

				BaseField( Uint16 dimx_in , Uint16 dimy_in, Uint16 dimz_in ):dimx(dimx_in), dimy(dimy_in), dimz(dimz_in){}

				/*! @return  extend of the field in x-direction.
				*/
				virtual Uint16 GetDimX(){ return dimx; }

				/*! @return  extend of the field in y-direction.
				*/
				virtual Uint16 GetDimY(){ return dimy; }

				/*! @return  extend of the field in z-direction.
				*/
				virtual Uint16 GetDimZ(){ return dimz; }

			protected:
				Uint16 dimx;
				Uint16 dimy;
				Uint16 dimz;      
			};

			/*! @class FieldOfScalar
			@brief This is a class representing a field of scalar values
			@todo uses UnsignedIndexList and other types from legacy code. Needs to be reimplemented. Close connection to 
			legacy mask need to be resolved.
			*/
			template <class TYPE>
			class FieldOfScalar : public BaseField
			{
			public:
				/*! @param dimx_in field extend in direction x.
				@param dimy_in field extend in direction y.
				@param dimz_in field extend in direction z.
				*/
				FieldOfScalar( Uint16 dimx_in , Uint16 dimy_in, Uint16 dimz_in ): BaseField( dimx_in , dimy_in , dimz_in )
				{
					create();
				}

				~FieldOfScalar(){
					clear();
				}

				typedef TYPE type;

				/*! @brief Get the data stored in this FieldOfScalar at index (x,y,z)
				@return  value of interest
				@exception IndexOutOfBoundsException
				*/
				inline TYPE GetData( Uint16 x, Uint16 y, Uint16 z );

				/*! @brief Set all values to zero.
				*/
				void SetZero();

				/*! @brief Set value at index (x,y,z) to value.
				@exception IndexOutOfBoundsException
				*/
				inline void PutData( Uint16 x, Uint16 y, Uint16 z , TYPE value ); 

				/*! @brief Check whether a 3D block of the length block_length with upper left corner (x,y,z) has the 
				intensity of value.
				@return True if the entire block does have the value of interest.
				*/
				bool CheckBlockSameIntensity( Uint16 x, Uint16 y, Uint16 z , Uint16 block_length , TYPE value );

				/*! @brief Set 3D block of the length block_length with upper left corner (x,y,z) to the value of input 
				parameter "value".
				*/      
				void SetBlockThisIntensity( Uint16 x, Uint16 y, Uint16 z , Uint16 block_length , TYPE value );

				/*! @brief This function investigates a specific area of the field, given by the parameter av. Those voxels 
				that are yet not clearly determined to be located inside or outside the contour and that are not part of 
				the border region are of interest. Depending on the neighbours of these voxels they are now defined as 
				inside or outside, if possible.

				@param av Specifies the region of interest within the field.
				@param indexListInside The function fills this list with voxels that are determined inside during this 
				procedure.
				@param indexListOutside The function fills this list with voxels that are determined outside during this 
				procedure.
				@param unclear_intensity Intensity or value (does not necessarily have to be an intensity) that 
				characterizes uncertainty with respect to association (inside/outside).
				@param inside_intensity  Intensity or value (does not necessarily have to be an intensity) that 
				characterizes inside.
				@param outside_intensity  Intensity or value (does not necessarily have to be an intensity) that 
				characterizes outside.
				*/
				void GetBorderRegion( masks::legacy::area_vector av, 
					std::vector< masks::legacy::UnsignedIndexList >& indexListInside , 
					std::vector< masks::legacy::UnsignedIndexList >& indexListOutside, TYPE unclear_intensity, 
					TYPE inside_intensity , TYPE outside_intensity );


				/*! @returns true in case one of the voxels within the regarded block does have the value "value".
				@param x x-position of the upper left corner of the block under investigation.
				@param y y-position of the upper left corner of the block under investigation.
				@param z z-position of the upper left corner of the block under investigation.
				@param block_length lenght of the block that is processed.
				*/      
				bool CheckOneOutOfThisBlockHasSameIntensity( Uint16 x, Uint16 y, Uint16 z , Uint16 block_length , TYPE value );

			private:
				/*! Return to initial state.
				*/
				void clear();

				/*! Initialize all.
				*/
				void create();

				TYPE* data;

				/*! Number of values within a slice.
				*/
				int slice_size;
				/*! Number of values within a raw.
				*/
				Uint16 raw_size;
				/*! Number of values within the field.
				*/
				int cube_size;
			}; 

			template <class TYPE>
			void FieldOfScalar<TYPE>::SetZero(){
				int size = ( dimz * dimy * dimx ) * sizeof(TYPE);
				memset( data , 0 , size );
			}

			template <class TYPE>
			inline TYPE FieldOfScalar<TYPE>::GetData( Uint16 x, Uint16 y, Uint16 z ){
				TYPE val;
				int offset;

				offset = ( z * slice_size + y * raw_size + x ) ;

				if(offset<0)
				{
					throw core::IndexOutOfBoundsException( "  Out of range ! " );
				}

				if( offset <= ( cube_size - 1 ) )
				{
					val = *( (TYPE*) data + offset );
					return val;
				}
				else{
					throw core::IndexOutOfBoundsException( "  Out of range ! " );  
				}
			}

			template <class TYPE>
			inline void FieldOfScalar<TYPE>::PutData( Uint16 x, Uint16 y, Uint16 z, TYPE value ){
				int offset;

				if( x >= dimx )
				{
					throw core::IndexOutOfBoundsException( "  Out of range ! " );
				}
				if( y >= dimy )
				{
					throw core::IndexOutOfBoundsException( "  Out of range ! " );
				}
				if( z >= dimz )
				{
					throw core::IndexOutOfBoundsException( "  Out of range ! " );
				}

				offset = ( z * slice_size + y * raw_size + x ) ;

				if(offset<0)
				{
					throw core::IndexOutOfBoundsException( "  Out of range ! " );
				}

				if( offset <= ( cube_size - 1 ) )
				{
					*( (TYPE*) data + offset ) = value;
				}     
				else{
					throw core::IndexOutOfBoundsException( "  Out of range ! " );
				}
			}

			template <class TYPE>
			void FieldOfScalar<TYPE>::clear(){
				free(data); 
			}

			template <class TYPE>
			void FieldOfScalar<TYPE>::create(){
				int size = ( dimz * dimy * dimx ) * sizeof(TYPE);      
				data = (type*) malloc( size );
				slice_size = dimy * dimx;
				raw_size = dimx;
				cube_size = dimy * dimx * dimz;

				memset( data , 0, cube_size * sizeof(TYPE) );
			}

			template <class TYPE>
			bool FieldOfScalar<TYPE>::CheckBlockSameIntensity( Uint16 x, Uint16 y, Uint16 z , Uint16 block_length , TYPE value ){
				if( x >= dimx )
				{
					assert(0);
				}
				if( y >= dimy )
				{
					assert(0);
				}
				if( z >= dimz )
				{
					assert(0);
				}

				Uint16 x_block_length = block_length;
				if( (  (dimx-1) - x ) <=  x_block_length )
				{
					x_block_length = (  (dimx-1) - x );     
				}
				Uint16 y_block_length = block_length; 
				if( (  (dimy-1) - y ) <=  y_block_length ) 
				{
					y_block_length = (  (dimy-1) - y );
				}

				TYPE* local_data_pointer;

				for( int pos = 0 ; pos <  y_block_length ; pos++ ){

					int offset = ( z * slice_size + ( y + pos ) * raw_size + x ) ;

					if( offset <= ( cube_size - 1 ) )
					{
						local_data_pointer = (TYPE*) (data + offset);

						for( Uint16 counter = 0 ; counter < x_block_length ; counter++ ){

							if( *(local_data_pointer) != value )
							{
								return false;
							} 
							local_data_pointer++;
						}
					} // if offset
					else{
						assert(0);
					}
				} // for pos       
				return true;
			}

			template <class TYPE>
			void FieldOfScalar<TYPE>::SetBlockThisIntensity( Uint16 x, Uint16 y, Uint16 z , Uint16 block_length , TYPE value ){
				if( x >= dimx )
				{
					assert(0);
				}
				if( y >= dimy )
				{
					assert(0);
				}
				if( z >= dimz )
				{
					assert(0);
				}

				Uint16 x_block_length = block_length;
				if( (  (dimx-1) - x ) <=  x_block_length ) 
				{
					x_block_length = (  (dimx-1) - x );   
				}
				Uint16 y_block_length = block_length; 
				if( (  (dimy-1) - y ) <=  y_block_length ) 
				{
					y_block_length = (  (dimy-1) - y );
				}

				TYPE* local_data_pointer;

				for( int pos = 0 ; pos <  y_block_length ; pos++ ){
					int offset = ( z * slice_size + ( y + pos ) * raw_size + x ) ;

					if( offset <= ( cube_size - 1 ) )
					{
						local_data_pointer = (TYPE*) (data + offset);

						for( Uint16 counter = 0 ; counter < x_block_length ; counter++ ){
							*(local_data_pointer) = value;
							local_data_pointer++;
						}
					} // if offset
					else{
						assert(0);
					}
				} // for pos   
			}

			template <class TYPE>
			void FieldOfScalar<TYPE>::GetBorderRegion( masks::legacy::area_vector av, 
				std::vector< masks::legacy::UnsignedIndexList >& indexListInside , 
				std::vector< masks::legacy::UnsignedIndexList >& indexListOutside, TYPE unclear_intensity, TYPE inside_intensity ,
				TYPE outside_intensity ){
					for( GridIndexType z = 0 ; z < dimz  ; z++ ){  
						for( GridIndexType y = av.at(z).index_begin.y ; y <= av.at(z).index_end.y ; y++ ){  
							for( GridIndexType x = av.at(z).index_begin.x ; x <= av.at(z).index_end.x ; x++ ){

								TYPE* data_pointer; 
								TYPE* data_pointer_check_neighbour;

								int offset = ( z * slice_size + y * raw_size + x ) ;

								data_pointer = (TYPE*) (data + offset);            

								if( *data_pointer == unclear_intensity )
								{
									bool its_inside = false;
									bool its_outside = false;

									if( x > av.at(z).index_begin.x )
									{
										int offset_neighbour = ( z * slice_size + y * raw_size + (x-1) ) ;
										data_pointer_check_neighbour = (TYPE*) (data + offset_neighbour);
										if( *data_pointer_check_neighbour  == inside_intensity )its_inside = true;
										if( *data_pointer_check_neighbour  == outside_intensity)its_outside = true;
									}  

									if( x < av.at(z).index_end.x )
									{
										int offset_neighbour = ( z * slice_size + y * raw_size + (x+1) ) ;
										data_pointer_check_neighbour = (TYPE*) (data + offset_neighbour);
										if( *data_pointer_check_neighbour  == inside_intensity ) its_inside = true;
										if( *data_pointer_check_neighbour  == outside_intensity) its_outside = true;
									}                    

									if( y > av.at(z).index_begin.y )
									{
										int offset_neighbour = ( z * slice_size + (y-1) * raw_size + x ) ;
										data_pointer_check_neighbour = (TYPE*) (data + offset_neighbour);
										if( *data_pointer_check_neighbour  == inside_intensity )its_inside = true;
										if( *data_pointer_check_neighbour  == outside_intensity)its_outside = true;
									}  

									if( y < av.at(z).index_end.y )
									{
										int offset_neighbour = ( z * slice_size + (y+1) * raw_size + x ) ;
										data_pointer_check_neighbour = (TYPE*) (data + offset_neighbour);                   
										if( *data_pointer_check_neighbour  == inside_intensity ) its_inside = true;
										if( *data_pointer_check_neighbour  == outside_intensity) its_outside = true;
									}

									if( its_inside && its_outside )
									{
										assert(0);
									}
									if( its_inside || its_outside )
									{
										masks::legacy::LegacyUnsignedIndex3D index = {x,y,z};

										if( its_inside )
										{
											indexListInside.at(z).push_back(index);
										}
										if( its_outside )
										{
											indexListOutside.at(z).push_back(index);
										}
									}  
								}
							} 
						}
					}
			}

			template <class TYPE>
			bool FieldOfScalar<TYPE>::CheckOneOutOfThisBlockHasSameIntensity( Uint16 x, Uint16 y, Uint16 z , Uint16 block_length ,
				TYPE value ){
					if( x >= dimx )
					{
						assert(0);
					}
					if( y >= dimy )
					{
						assert(0);
					}
					if( z >= dimz )
					{
						assert(0);
					}

					bool its_fine = false;    

					Uint16 x_block_length = block_length;
					if( (  (dimx-1) - x ) <=  x_block_length )
					{
						x_block_length = (  (dimx-1) - x );     
					}
					Uint16 y_block_length = block_length; 
					if( (  (dimy-1) - y ) <=  y_block_length )
					{
						y_block_length = (  (dimy-1) - y );
					}

					TYPE* local_data_pointer;

					for( int pos = 0 ; pos <  y_block_length ; pos++ ){

						int offset = ( z * slice_size + ( y + pos ) * raw_size + x ) ;

						if( offset <= ( cube_size - 1 ) )
						{
							local_data_pointer = (TYPE*) data + offset;

							for( int counter = 0 ; counter < x_block_length ; counter++ ){

								if( *local_data_pointer == value )
								{
									its_fine = true;
								}
								local_data_pointer++;
							}
						} // if offset
						else{
							assert(0);
						}
					} // for pos      
					return its_fine;
			}

			/*! @class FieldOfPointer
			@brief This is a class representing a field of pointers to whatever. 
			*/
			template <class TYPE>
			class FieldOfPointer : public BaseField
			{
			public:
				FieldOfPointer( Uint16 dimx_in , Uint16 dimy_in, Uint16 dimz_in ): BaseField( dimx_in , dimy_in , dimz_in )
				{
					create();
				}

				~FieldOfPointer(){
					clear();
				}

				typedef TYPE type;

				/*! @brief Get the data stored in this FieldOfPointer at index (x,y,z)
				@see FieldOfScalar. 
				*/
				inline TYPE GetData( Uint16 x, Uint16 y, Uint16 z );

				/*! @brief Set all entries to Null
				@see FieldOfScalar. Here zero is NULL.       
				*/
				void SetZero();

				/*! @brief Set value at index (x,y,z)
				@see FieldOfScalar.
				*/
				inline void PutData( Uint16 x, Uint16 y, Uint16 z , TYPE value ); 

			private:	

				void clear();

				/*! Initialize all.
				*/    
				void create();

				TYPE* data;

				/*! Number of values within a slice.
				*/
				int slice_size;
				/*! Number of values in a raw.
				*/        
				Uint16 raw_size;
				/*! Number of values within a cube.
				*/       
				int cube_size;
			}; 

			template <class TYPE>
			void FieldOfPointer<TYPE>::SetZero(){
				for( int offset = 0 ; offset < cube_size ; offset++ ){
					delete *( (TYPE*) data + offset );
					*( (TYPE*) data + offset ) = NULL;
				}  
			}

			template <class TYPE>
			inline TYPE FieldOfPointer<TYPE>::GetData( Uint16 x, Uint16 y, Uint16 z ){
				TYPE val;
				int offset;

				offset = ( z * slice_size + y * raw_size + x ) ;

				if( offset <= ( cube_size - 1 ) )
				{
					val = *( (TYPE*) data + offset ); 
					return val;
				}
				else{
					throw core::IndexOutOfBoundsException( "  Out of range ! " );
				}
			}


			template <class TYPE>
			inline void FieldOfPointer<TYPE>::PutData( Uint16 x, Uint16 y, Uint16 z, TYPE value ){
				int offset;

				offset = ( z * slice_size + y * raw_size + x ) ;

				if( offset <= ( cube_size - 1 ) )
				{
					delete *( (TYPE*) data + offset );
					*( (TYPE*) data + offset ) = value;
				}     
				else{
					throw core::IndexOutOfBoundsException( "  Out of range ! " );
				}
			}

			template <class TYPE>
			void FieldOfPointer<TYPE>::clear(){
				for( int offset = 0 ; offset < cube_size ; offset++ ) delete *( (TYPE*) data + offset );
				free(data); 
			}

			template <class TYPE>
			void FieldOfPointer<TYPE>::create(){
				int size = ( dimz * dimy * dimx ) * sizeof(TYPE);      
				data = (type*) malloc( size );
				slice_size = dimy * dimx;
				raw_size = dimx;
				cube_size = dimy * dimx * dimz;
				for( int offset = 0 ; offset < cube_size ; offset++ ){
					*( (TYPE*) data + offset ) =   NULL;
				}
			}
		}
	}
}


#endif 




