/*****************************************************************************
 * example.cpp
 *****************************************************************************/

#include "adi_initialize.h"
#include "example.h"

int main(void)
{
	/**
	 * Initialize managed drivers and/or services that have been added to 
	 * the project.
	 * @return zero on success 
	 */
	adi_initComponents();
	
	/* Begin adding your custom code here */

	return 0;
}

