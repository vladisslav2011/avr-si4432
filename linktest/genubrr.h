/*

    Copyright 2012 vlad <vladisslav@inbox.ru>

    This file is part of accounting utilities.

    This is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

   It is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this file. If not, see http://www.gnu.org/licenses/.

*/
#define GENUBRR_D(baud,u2x) (F_CPU/((u2x==0)?16:8)/baud-1)
#define GENUBRR_1(baud,u2x) ((GENUBRR_D(baud,u2x)>4095)?4095:GENUBRR_D(baud,u2x))
#define GEN_TBAUD(ubrr,u2x) (F_CPU/((u2x==0)?16:8)/(ubrr+1))
#define BAUD_ERROR_U(tbaud,baud) ((double)tbaud/(double)baud - 1.0)
#define BAUD_ERROR(tbaud,baud) ((BAUD_ERROR_U(tbaud,baud)>=0.0)?BAUD_ERROR_U(tbaud,baud):-BAUD_ERROR_U(tbaud,baud))
#define GENUBRR(baud)	((BAUD_ERROR(GEN_TBAUD(GENUBRR_1(baud,1),1),baud)>\
						BAUD_ERROR(GEN_TBAUD(GENUBRR_1(baud,0),0),baud))?\
						GENUBRR_1(baud,0):\
						GENUBRR_1(baud,1))

#define GENU2X(baud)	((BAUD_ERROR(GEN_TBAUD(GENUBRR_1(baud,1),1),baud)>\
						BAUD_ERROR(GEN_TBAUD(GENUBRR_1(baud,0),0),baud))?\
						(0<<U2X):\
						(1<<U2X))
#define GENUBRR_T(baud) {ubrr:GENUBRR(baud),u2x:GENU2X(baud)}
