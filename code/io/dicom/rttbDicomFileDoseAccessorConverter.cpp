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
// @version $Revision: 747 $ (last changed revision)
// @date    $Date: 2014-09-17 12:01:00 +0200 (Mi, 17 Sep 2014) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/


#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include "rttbDicomFileDoseAccessorConverter.h"
#include "rttbDicomIODDoseAccessorConverter.h"
#include "rttbInvalidDoseException.h"
#include "rttbGeometricInfo.h"
#include "rttbGeometricInfo.h"
#include "rttbGenericDoseIterator.h"
#include "rttbDoseStatistics.h"

namespace rttb
{
	namespace io
	{
		namespace dicom
		{

			DicomFileDoseAccessorConverter::DicomFileDoseAccessorConverter(DoseAccessorPointer aAccessor, DICOMRTFileNameString aFileName)
				:_fileName(aFileName)
			{
				setDoseAccessor(aAccessor);
				_doseIOD = boost::make_shared<DRTDoseIOD>();
				_dataset = _fileformat.getDataset();
			}

			bool DicomFileDoseAccessorConverter::process()
			{
				std::string s;
				OFCondition status;

				/* Prepare dcmtk */				
				DcmItem *dcm_item = 0;

				//get geometric info
				rttb::core::GeometricInfo geometricInfo = _doseAccessor->getGeometricInfo(); 


				/* ----------------------------------------------------------------- */
				/*     Part 1  -- General header                                     */
				/* ----------------------------------------------------------------- */
				OFString CreationUID(_doseAccessor->getDoseUID().c_str());
				_dataset->putAndInsertString (DCM_ImageType, 
					"DERIVED\\SECONDARY\\REFORMATTED");
				_dataset->putAndInsertOFStringArray(DCM_InstanceCreationDate, 
					"");//Creation Date
				_dataset->putAndInsertOFStringArray(DCM_InstanceCreationTime, 
					"");//Creation Time
				_dataset->putAndInsertOFStringArray(DCM_InstanceCreatorUID, 
					CreationUID);
				_dataset->putAndInsertString (DCM_SOPClassUID, UID_RTDoseStorage);
				_dataset->putAndInsertString (DCM_SOPInstanceUID, 
					_doseAccessor->getDoseUID().c_str());
				_dataset->putAndInsertOFStringArray (DCM_StudyDate, 
					"");
				_dataset->putAndInsertOFStringArray (DCM_StudyTime, 
					"");
				_dataset->putAndInsertOFStringArray (DCM_AccessionNumber, "");
				_dataset->putAndInsertOFStringArray (DCM_Modality, "RTDOSE");
				_dataset->putAndInsertString (DCM_Manufacturer, "RTToolbox");
				_dataset->putAndInsertString (DCM_InstitutionName, "");
				_dataset->putAndInsertString (DCM_ReferringPhysicianName, "");
				_dataset->putAndInsertString (DCM_StationName, "");
				_dataset->putAndInsertString (DCM_ManufacturerModelName, "RTToolbox");


				/* (0008,1140) DCM_ReferencedImageSequence -- MIM likes this */
				dcm_item = 0;
				_dataset->findOrCreateSequenceItem (
					DCM_ReferencedImageSequence, dcm_item, -2);
				dcm_item->putAndInsertString (DCM_ReferencedSOPClassUID,
					UID_CTImageStorage);
				dcm_item->putAndInsertString (DCM_ReferencedSOPInstanceUID,
					"");//ct_series_uid?

				_dataset->putAndInsertString (DCM_PatientName, "");
				_dataset->putAndInsertString (DCM_PatientID, "");
				_dataset->putAndInsertString (DCM_PatientBirthDate, "");
				_dataset->putAndInsertString (DCM_PatientSex, "O");
				_dataset->putAndInsertString (DCM_SliceThickness, boost::lexical_cast<std::string>(geometricInfo.getSliceThickness()).c_str());
				_dataset->putAndInsertString (DCM_SoftwareVersions,
					"");
				_dataset->putAndInsertString (DCM_StudyInstanceUID, 
					"");
				_dataset->putAndInsertString (DCM_SeriesInstanceUID, 
					"");
				_dataset->putAndInsertString (DCM_StudyID, "10001");
				_dataset->putAndInsertString (DCM_SeriesNumber, "");
				_dataset->putAndInsertString (DCM_InstanceNumber, "1");

				s = string_format ("%g\\%g\\%g", geometricInfo.getImagePositionPatient().x(), 
					geometricInfo.getImagePositionPatient().y(), geometricInfo.getImagePositionPatient().z());
				/* GCS FIX: PatientOrientation */
				_dataset->putAndInsertString (DCM_PatientOrientation, "L/P");
				_dataset->putAndInsertString (DCM_ImagePositionPatient, s.c_str());
				s = string_format ("%g\\%g\\%g\\%g\\%g\\%g",
					geometricInfo.getImageOrientationRow().x(),
					geometricInfo.getImageOrientationRow().y(),
					geometricInfo.getImageOrientationRow().z(),
					geometricInfo.getImageOrientationColumn().x(),
					geometricInfo.getImageOrientationColumn().y(),
					geometricInfo.getImageOrientationColumn().z());
				_dataset->putAndInsertString (DCM_ImageOrientationPatient, s.c_str());
				_dataset->putAndInsertString (DCM_FrameOfReferenceUID, 
					"");//FrameOfReferenceUID?

				_dataset->putAndInsertString (DCM_SamplesPerPixel, "1");
				_dataset->putAndInsertString (DCM_PhotometricInterpretation, "MONOCHROME2");
				s = string_format ("%d", geometricInfo.getNumSlices());
				_dataset->putAndInsertString (DCM_NumberOfFrames, s.c_str());

				/* GCS FIX: Add FrameIncrementPointer */
				_dataset->putAndInsertString (DCM_FrameIncrementPointer, 
					"(3004,000c)");

				_dataset->putAndInsertUint16 (DCM_Rows, geometricInfo.getNumRows());
				_dataset->putAndInsertUint16 (DCM_Columns, geometricInfo.getNumColumns());
				s = string_format ("%g\\%g", 
					geometricInfo.getSpacing()(1), geometricInfo.getSpacing()(0));
				_dataset->putAndInsertString (DCM_PixelSpacing, s.c_str());

				_dataset->putAndInsertString (DCM_BitsAllocated, "32");
				_dataset->putAndInsertString (DCM_BitsStored, "32");
				_dataset->putAndInsertString (DCM_HighBit, "31");
				/*if (dose_metadata 
				&& dose_metadata->get_metadata(0x3004, 0x0004) == "ERROR")
				{
				_dataset->putAndInsertString (DCM_PixelRepresentation, "1");
				} else {
				_dataset->putAndInsertString (DCM_PixelRepresentation, "0");
				}*/

				_dataset->putAndInsertString (DCM_DoseUnits, "GY");
				/*dcmtk_copy_from_metadata (_dataset, dose_metadata, 
				DCM_DoseType, "PHYSICAL");*/
				_dataset->putAndInsertString (DCM_DoseSummationType, "PLAN");

				s = std::string ("0");
				for (int i = 1; i < geometricInfo.getNumSlices(); i++) {
					s += string_format ("\\%g",  i * geometricInfo.getSpacing()(2));//*dose_volume->direction_cosines[8]? What is dose_volume->direction_cosines[8]
				}
				_dataset->putAndInsertString (DCM_GridFrameOffsetVector, s.c_str());

				/* GCS FIX:
				Leave ReferencedRTPlanSequence empty (until I can cross reference) */

				/* We need to convert image to uint16_t, but first we need to 
				scale it so that the maximum dose fits in a 16-bit unsigned 
				integer.  Compute an appropriate scaling factor based on the 
				maximum dose. */


				/* Find the maximum value in the image */
				boost::shared_ptr<rttb::core::GenericDoseIterator> spTestDoseIterator = boost::make_shared<rttb::core::GenericDoseIterator>(_doseAccessor);
				rttb::core::GenericDoseIterator::DoseIteratorPointer spDoseIterator (spTestDoseIterator);
				rttb::algorithms::DoseStatistics doseStat (spDoseIterator);
				boost::shared_ptr< std::vector<std::pair<DoseTypeGy,VoxelGridID> > > myResultPairs = 
					boost::make_shared< std::vector<std::pair<DoseTypeGy,VoxelGridID> > >();
				rttb::algorithms::DoseStatistics::ResultListPointer spMyResultPairs(myResultPairs);
				double minDose = doseStat.getMinimum(myResultPairs);
				double maxDose = doseStat.getMaximum(myResultPairs);

				/* Find scale factor */
				float dose_scale;
				dose_scale = maxDose /PixelDataMaxValue;

				/* Scale the image and add scale factor to _dataset */
				s = string_format ("%g", dose_scale);
				_dataset->putAndInsertString (DCM_DoseGridScaling, s.c_str());

				/* (300c,0002) ReferencedRTPlanSequence -- for future expansion */
				dcm_item = 0;
				_dataset->findOrCreateSequenceItem (
					DCM_ReferencedRTPlanSequence, dcm_item, -2);
				dcm_item->putAndInsertString (DCM_ReferencedSOPClassUID,
					UID_RTPlanStorage);
				dcm_item->putAndInsertString (DCM_ReferencedSOPInstanceUID,
					"");//ReferencedSOPInstanceUID??

				/* (300c,0060) DCM_ReferencedStructureSetSequence -- MIM likes this */
				dcm_item = 0;
				_dataset->findOrCreateSequenceItem (
					DCM_ReferencedStructureSetSequence, dcm_item, -2);
				dcm_item->putAndInsertString (DCM_ReferencedSOPClassUID,
					UID_RTStructureSetStorage);
				dcm_item->putAndInsertString (DCM_ReferencedSOPInstanceUID,
					"");//ReferencedSOPInstanceUID??

				/* Convert image bytes to integer, then add to _dataset */
				Uint16* pixelData;
				int pixelCount = geometricInfo.getNumRows() * geometricInfo.getNumColumns() * geometricInfo.getNumSlices();
				pixelData = new Uint16[pixelCount];
				for(unsigned int i=0; i<pixelCount; ++i){
					double doseValue = _doseAccessor->getDoseAt(i);
					double pixelValue = doseValue/dose_scale;
					if(pixelValue > PixelDataMaxValue){
						pixelValue = PixelDataMaxValue;
					}
					
					pixelData[i] =boost::numeric_cast<Uint16>(pixelValue);
				}

				status = _dataset->putAndInsertUint16Array (DCM_PixelData, 
					pixelData, pixelCount);
		
				if(!status.good()){
					throw core::InvalidDoseException("Error: put and insert pixel data failed!");
				}

				return true;
			}

			void DicomFileDoseAccessorConverter::writeDicomDoseFile(){
				OFCondition status;

				status = _fileformat.saveFile (_fileName.c_str(), EXS_LittleEndianExplicit);
				if (status.bad()) {
					std::cerr << "Error: cannot write DICOM RTDOSE!" << std::endl;
				}

			}

			std::string DicomFileDoseAccessorConverter::string_format (const char *fmt, va_list ap)
			{
				int size=100;
				std::string str;
				while (1) {
					str.resize(size);
					va_list ap_copy = ap; //va_list ap_copy; va_copy (ap_copy, ap); va_copy() is supported starting in Visual Studio 2013.
					int n = vsnprintf((char *)str.c_str(), size, fmt, ap_copy);
					va_end (ap_copy);
					if (n > -1 && n < size) {
						str = std::string (str.c_str());  /* Strip excess padding */
						return str;
					}
					if (n > -1)
						size=n+1;
					else
						size*=2;
				}
			}

			std::string DicomFileDoseAccessorConverter::string_format (const char *fmt, ...)
			{
				va_list ap;
				va_start (ap, fmt);
				std::string string = string_format (fmt, ap);
				va_end (ap);
				return string;
			}


		}//end namespace itk
	}//end namespace io
}//end namespace rttb

