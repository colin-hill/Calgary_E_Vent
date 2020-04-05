/* Functions for handling the VC Mode of the ventilator.
 */

#ifndef VCMode_h
#define VCMode_h


/* States for the VC Mode state machine.

   TODO: directions to state machine diagram.
 */
enum vcModeStates {
                   VCStart,
                   VCInhale,
                   VCInhaleCommand,
                   VCPeak,
                   VCExhale,
                   VCReset
};

#endif
