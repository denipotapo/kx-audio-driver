// kX Driver
// Copyright (c) Eugene Gavrilov, 2001-2014.
// All rights reserved

// NOTE: the same code is in NetBSD source (under NetBSD license)

/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */


#include "kx.h"

dword kx_srToPitch(dword sampleRate)
{
    int i;

    static dword logMagTable[128] = {
        0x00000, 0x02dfc, 0x05b9e, 0x088e6, 0x0b5d6, 0x0e26f, 0x10eb3, 0x13aa2,
        0x1663f, 0x1918a, 0x1bc84, 0x1e72e, 0x2118b, 0x23b9a, 0x2655d, 0x28ed5,
        0x2b803, 0x2e0e8, 0x30985, 0x331db, 0x359eb, 0x381b6, 0x3a93d, 0x3d081,
        0x3f782, 0x41e42, 0x444c1, 0x46b01, 0x49101, 0x4b6c4, 0x4dc49, 0x50191,
        0x5269e, 0x54b6f, 0x57006, 0x59463, 0x5b888, 0x5dc74, 0x60029, 0x623a7,
        0x646ee, 0x66a00, 0x68cdd, 0x6af86, 0x6d1fa, 0x6f43c, 0x7164b, 0x73829,
        0x759d4, 0x77b4f, 0x79c9a, 0x7bdb5, 0x7dea1, 0x7ff5e, 0x81fed, 0x8404e,
        0x86082, 0x88089, 0x8a064, 0x8c014, 0x8df98, 0x8fef1, 0x91e20, 0x93d26,
        0x95c01, 0x97ab4, 0x9993e, 0x9b79f, 0x9d5d9, 0x9f3ec, 0xa11d8, 0xa2f9d,
        0xa4d3c, 0xa6ab5, 0xa8808, 0xaa537, 0xac241, 0xadf26, 0xafbe7, 0xb1885,
        0xb3500, 0xb5157, 0xb6d8c, 0xb899f, 0xba58f, 0xbc15e, 0xbdd0c, 0xbf899,
        0xc1404, 0xc2f50, 0xc4a7b, 0xc6587, 0xc8073, 0xc9b3f, 0xcb5ed, 0xcd07c,
        0xceaec, 0xd053f, 0xd1f73, 0xd398a, 0xd5384, 0xd6d60, 0xd8720, 0xda0c3,
        0xdba4a, 0xdd3b4, 0xded03, 0xe0636, 0xe1f4e, 0xe384a, 0xe512c, 0xe69f3,
        0xe829f, 0xe9b31, 0xeb3a9, 0xecc08, 0xee44c, 0xefc78, 0xf148a, 0xf2c83,
        0xf4463, 0xf5c2a, 0xf73da, 0xf8b71, 0xfa2f0, 0xfba57, 0xfd1a7, 0xfe8df
    };

    static char logSlopeTable[128] = {
        0x5c, 0x5c, 0x5b, 0x5a, 0x5a, 0x59, 0x58, 0x58,
        0x57, 0x56, 0x56, 0x55, 0x55, 0x54, 0x53, 0x53,
        0x52, 0x52, 0x51, 0x51, 0x50, 0x50, 0x4f, 0x4f,
        0x4e, 0x4d, 0x4d, 0x4d, 0x4c, 0x4c, 0x4b, 0x4b,
        0x4a, 0x4a, 0x49, 0x49, 0x48, 0x48, 0x47, 0x47,
        0x47, 0x46, 0x46, 0x45, 0x45, 0x45, 0x44, 0x44,
        0x43, 0x43, 0x43, 0x42, 0x42, 0x42, 0x41, 0x41,
        0x41, 0x40, 0x40, 0x40, 0x3f, 0x3f, 0x3f, 0x3e,
        0x3e, 0x3e, 0x3d, 0x3d, 0x3d, 0x3c, 0x3c, 0x3c,
        0x3b, 0x3b, 0x3b, 0x3b, 0x3a, 0x3a, 0x3a, 0x39,
        0x39, 0x39, 0x39, 0x38, 0x38, 0x38, 0x38, 0x37,
        0x37, 0x37, 0x37, 0x36, 0x36, 0x36, 0x36, 0x35,
        0x35, 0x35, 0x35, 0x34, 0x34, 0x34, 0x34, 0x34,
        0x33, 0x33, 0x33, 0x33, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x31, 0x31, 0x31, 0x31, 0x31, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f
    };

    if(sampleRate==0)
        return 0;   /* Bail out if no leading "1" */

    sampleRate *= 11185;    /* Scale 48000 to 0x20002380 */

    for(i=31;i>0;i--)
    {
        if(sampleRate & 0x80000000) 
        {   /* Detect leading "1" */
            return (dword) (((int) (i - 15) << 20) +
                      logMagTable[0x7f & (sampleRate >> 24)] +
                      (0x7f & (sampleRate >> 17)) * logSlopeTable[0x7f & (sampleRate >> 24)]);
        }
        sampleRate = sampleRate << 1;
    }

    return 0;       /* Should never reach this point */
}

dword kx_samplerate_to_linearpitch(dword samplingrate)
{
    samplingrate = (samplingrate << 8) / 375;   // *=0.34133.. = (4096/12)/1000
    return (samplingrate >> 1) + (samplingrate & 1);
}


// param is controlled using KX_MAX_VOLUME.KX_MIN_VOLUME
// ret is from 0 to max linear
dword calc_volume(kx_hw *hw,dword value,dword max)
{
    // 0x80000000 - DSound/WDM mute
    if(value==0x80000000U || value==KX_MIN_VOLUME)
    {
     value=0x0;
     }
    else if(value==KX_MAX_VOLUME)
    {
     value=max;
    }
    else
    {
           if((int)value<(int)KX_MIN_VOLUME)
             value=(dword)KX_MIN_VOLUME;
           if((int)value>(int)KX_MAX_VOLUME)
             value=(dword)KX_MAX_VOLUME;

           kx_fpu_state state;
           hw->cb.save_fpu_state(&state);

           // val=10^((val/0x10000)/20);
           if(max!=0x80000000) // pre-3543
            value=(dword)(kx_pow10((((double)(int)value)*(7.62939453125e-7)))*(double)max);
           else // 3543
            value=(dword)(int)(-kx_pow10((((double)(int)value)*(7.62939453125e-7)))*(double)max);

           hw->cb.rest_fpu_state(&state);
    }
    return value;
}

/*
byte kx_fxamount_tbl_old[256]=
{
 0x0, 0x0, 0x0, 0x4, 0x8, 0xc, 0x10, 0x14,
 0x18, 0x1c, 0x24, 0x28, 0x2c, 0x2c, 0x30, 0x34,
 0x38, 0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c,
 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c,
 0x5e, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e,
 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e,
 0x7f, 0x80, 0x80, 0x81, 0x81, 0x82, 0x82, 0x83,
 0x84, 0x84, 0x85, 0x85, 0x86, 0x86, 0x86, 0x87,
 0x88, 0x88, 0x89, 0x89, 0x8a, 0x8a, 0x8b, 0x8b,
 0x8c, 0x8c, 0x8d, 0x8d, 0x8e, 0x8e, 0x8f, 0x8f,
 0x90, 0x90, 0x91, 0x91, 0x92, 0x92, 0x93, 0x93,
 0x94, 0x94, 0x95, 0x95, 0x96, 0x96, 0x97, 0x97,
 0x98, 0x98, 0x99, 0x99, 0x9a, 0x9a, 0x9b, 0x9b,
 0x9c, 0x9c, 0x9d, 0x9d, 0x9e, 0x9e, 0x9f, 0x9f,
 0x9f, 0xa0, 0xa0, 0xa0, 0xa0, 0xa1, 0xa1, 0xa1,
 0xa2, 0xa2, 0xa2, 0xa2, 0xa3, 0xa3, 0xa3, 0xa3,
 0xa4, 0xa4, 0xa4, 0xa4, 0xa5, 0xa5, 0xa5, 0xa5,
 0xa6, 0xa6, 0xa6, 0xa6, 0xa7, 0xa7, 0xa7, 0xa7,
 0xa8, 0xa8, 0xa8, 0xa8, 0xa9, 0xa9, 0xa9, 0xa9,
 0xaa, 0xaa, 0xaa, 0xaa, 0xab, 0xab, 0xab, 0xab,
 0xac, 0xac, 0xac, 0xac, 0xad, 0xad, 0xad, 0xad,
 0xae, 0xae, 0xae, 0xae, 0xaf, 0xaf, 0xaf, 0xb0,
 0xb0, 0xb0, 0xb0, 0xb0, 0xb1, 0xb1, 0xb1, 0xb2,
 0xb2, 0xb2, 0xb2, 0xb3, 0xb3, 0xb3, 0xb3, 0xb4,
 0xb4, 0xb4, 0xb4, 0xb5, 0xb5, 0xb5, 0xb5, 0xb6,
 0xb6, 0xb6, 0xb6, 0xb7, 0xb7, 0xb7, 0xb7, 0xb8,
 0xb8, 0xb8, 0xb8, 0xb9, 0xb9, 0xb9, 0xb9, 0xba,
 0xba, 0xba, 0xba, 0xbb, 0xbb, 0xbb, 0xbb, 0xbc,
 0xbc, 0xbc, 0xbc, 0xbd, 0xbd, 0xbd, 0xbd, 0xbe,
 0xbe, 0xbe, 0xbe, 0xbf, 0xbf, 0xbf, 0xbf, 0xc0
 };
*/

// 3543:
// -106.6667: -20*(32/6)
// 192 - log10 (linear / 256) * (-106.6667)

byte kx_fxamount_tbl[256]=
{
 // 0..224
0,0,0,18,32 /*31*/,42,50,57,63,69,74,78,82,86,89,93,
96,98,101,104,106,108,110,112,114,116,118,120,121,123,125,126,
128,129,130,132,133,134,136,137,138,139,140,141,142,143,144,145,
146,147,148,149,150,151,152,153,154,154,155,156,157,158,158,159,
160,160,161,162,163,163,164,165,165,166,167,167,168,168,169,170,
170,171,171,172,172,173,173,174,175,175,176,176,177,177,178,178,
179,179,180,180,180,181,181,182,182,183,183,184,184,184,185,185,
186,186,187,187,187,188,188,189,189,189,190,190,190,191,191,192,
192,192,193,193,193,194,194,194,195,195,195,196,196,196,197,197,
197,198,198,198,199,199,199,200,200,200,200,201,201,201,202,202,
202,203,203,203,203,204,204,204,204,205,205,205,206,206,206,206,
207,207,207,207,208,208,208,208,209,209,209,209,210,210,210,210,
211,211,211,211,212,212,212,212,213,213,213,213,213,214,214,214,
214,215,215,215,215,215,216,216,216,216,217,217,217,217,217,218,
218,218,218,218,219,219,219,219,219,220,220,220,220,220,221,221,
221,221,221,222,222,222,222,222,223,223,223,223,223,223,224,224
/*
 // 0..255
0,0,31,50,63,74,82,89,95,101,106,110,114,118,121,125,
128,130,133,136,138,140,142,144,146,148,150,152,153,155,157,158,
160,161,162,164,165,166,168,169,170,171,172,173,174,175,176,177,
178,179,180,181,182,183,184,185,186,186,187,188,189,190,190,191,
192,192,193,194,195,195,196,197,197,198,199,199,200,200,201,202,
202,203,203,204,204,205,205,206,207,207,208,208,209,209,210,210,
211,211,212,212,212,213,213,214,214,215,215,216,216,216,217,217,
218,218,219,219,219,220,220,221,221,221,222,222,222,223,223,224,
224,224,225,225,225,226,226,226,227,227,227,228,228,228,229,229,
229,230,230,230,231,231,231,232,232,232,232,233,233,233,234,234,
234,235,235,235,235,236,236,236,236,237,237,237,238,238,238,238,
239,239,239,239,240,240,240,240,241,241,241,241,242,242,242,242,
243,243,243,243,244,244,244,244,245,245,245,245,245,246,246,246,
246,247,247,247,247,247,248,248,248,248,249,249,249,249,249,250,
250,250,250,250,251,251,251,251,251,252,252,252,252,252,253,253,
253,253,253,254,254,254,254,254,255,255,255,255,255,255,255,255
*/
};

dword kx_sr_coeff(kx_hw *hw,dword sr)
{
  if(hw->card_frequency==48000)
   return sr;

  if(hw->card_frequency==44100)
  {
      if(sr==44100)
       return 48000;
      else
       return sr*48000/44100;
  }

  debug(DERR,"kx_sr_coeff: invalid sr [%d]\n",hw->card_frequency);
  return 48000;
}