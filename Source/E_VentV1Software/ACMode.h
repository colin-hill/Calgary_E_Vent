/* Functions for handling the AC Mode of the ventilator.

   AC Mode allows the patient to control the breathing of the ventilator.
 */

#ifndef ACMode_h
#define ACMode_h

/* States for the AC Mode state machine.

   TODO: directions to state machine diagram.
 */
enum acModeStates {
                   ACStart,
                   ACInhaleWait,
                   ACInhaleCommand,
                   ACInhale,
                   ACPeak,
                   ACExhale,
                   ACReset
};

#endif
