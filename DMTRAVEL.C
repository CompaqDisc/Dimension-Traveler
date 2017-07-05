/* 
*            Dimension Traveller: Copyright 2017 CompaqDisc
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_DIMS 10

typedef struct dimension {
    char name[12];
    int ratio;
};

int calc_ratio(int src_scl, int dest_scl, int src_coord) {
    int x;
    
    x = src_scl * src_coord;
    x = x / dest_scl;
    
    return x;
}

int sign(int x) {
    return (x > 0) - (x < 0);
}

int len(int x) {
    return (int) floor(log10(abs(x))) + 1 + ((sign(x) == -1) ? 1 : 0);
}

int maxlen(int a, int b, int c, int d) {
    int a_len = len(a);
    int b_len = len(b);
    int c_len = len(c);
    int d_len = len(d);
    
    return (int) (fmax(fmax(a_len, b_len), fmax(c_len, d_len)));
}

int main() {
    
    printf("DIMENSION TRAVELLER: COPYRIGHT 2017 COMPAQDISC\n\n");
    
    // Default dimensions.
    const char* defaults = "OVERWORLD   :1\nNETHER      :8\n";
    
    int dimc; // Dimension count.
    
    int origin; // Selection for dimension.
    int dest;   // Selection for dimension.
    
    int ox; // Origin X-coord
    int oz; // Origin Z-coord
    
    int dx; // Dest. X-coord
    int dz; // Dest. Z-coord
    
    int mlen; // Max length of ints.
    
    char dim_name[12];  // Temp holder for dimension name.
    int dim_ratio;      // Temp holder for dimension ratio.
    
    FILE* handle = NULL;
    FILE* test = NULL;
    
    // Try and open file.
    handle = fopen("DIMS.DAT", "r");
    
    // If exists on system:
    if (handle) {
        printf("Loading dimension data...\n");
    }
    
    // Does not exist, let's create a default one:
    else {
        
        // Close the invalid head handle.
        fclose(handle);
        printf("Creating defaults...\n");
        
        // Create a handle for writing, and write the defaults.
        handle = fopen("DIMS.DAT", "w");
        fwrite(defaults, strlen(defaults), 1, handle);
        
        // Close the write handle, and open one for reading.
        fclose(handle);
        handle = fopen("DIMS.DAT", "r");
    }
    
    // Get number of dimension entries.
    dimc = fscanf(handle, "%12c:%i\n", dim_name, &dim_ratio);
    
    // Seek back to start.
    fseek(handle, 0, SEEK_SET);
    
    // Reserve enough space for all entries.
    struct dimension dims[dimc];
    
    // Load all dimensions into dims.
    int i = 0;
    while (fscanf(handle, "%12c:%i\n", dim_name, &dim_ratio) > 0) {
        // Clear the last char, max 11 characters per dimension name,
        // followed by a trailing null.
        dim_name[11] = NULL;
        
        // Create new instance of a dimension struct.
        struct dimension dim;
        
        // Copy our name buffer into property 'name'
        strncpy(dim.name, dim_name, 12);
        
        // Set property 'ratio'
        dim.ratio = dim_ratio;
        
        // Add to array.
        dims[i] = dim;
        
        // Next loop.
        i++;
    }
    
    // Done reading!
    fclose(handle);
    
    // Did we acually load any dimensions?
    if (i != 0) {
        // Yes, print how many.
        printf("Loaded %i dimensions!\n\n", i);
    } else {
        // No, exit with error.
        printf("No dimensions loaded!\n");
        return 1;
    }
    
    // List all loaded dimensions.
    for (int i = 0; i < dimc; i++) {
        printf("\t%i: %s\n", i + 1, dims[i].name);
    }
    printf("\n");
    
    // Get origin.
    do {
        do {
            printf("Dimension of origin? (1-%i)\n> ", dimc);
        } while (scanf("%d", &origin) != 1);
    } while (!(origin <= dimc) && (origin != 0));
    // ^ Our origin must be one of our valid choices
    // (ie. Less than or equal to our dimension count
    //  and also non-zero.)
    
    // Decrement origin for use as an index into dims.
    origin--;
    
    // Inform user.
    printf("\n\tOrigin\t: [%s]\n\tScale\t: %i\n\n", 
        dims[origin].name, dims[origin].ratio);
    
    // Get destination.
    do {
        do {
            printf("Destination dimension? (1-%i)\n> ", dimc);
        } while (scanf("%d", &dest) != 1);
    } while (!(dest <= dimc) && (dest != 0));
    // ^ Our destination must be one of our valid choices
    // (ie. Less than or equal to our dimension count
    //  and also non-zero.)
    
    // Decrement dest for use as an index into dims.
    dest--;
    
    // Inform user.
    printf("\n\tDest.\t: [%s]\n\tScale\t: %i\n\n", 
        dims[dest].name, dims[dest].ratio);
    
    // Get input coordinates.
    do {
        printf("Please input coordinates in [%s] as (x,z)\n> ", 
            dims[origin].name);
    } while (scanf("%d,%d", &ox, &oz) != 2 );
    printf("\n");
    
    dx = calc_ratio(dims[origin].ratio, dims[dest].ratio, ox);
    dz = calc_ratio(dims[origin].ratio, dims[dest].ratio, oz);
    
    mlen = maxlen(ox, oz, dx, dz);
    
    printf("\tOrigin:\n\t\t%s: (%*d, %*d)\n\n\tDestination:\n\t\t%s: (%*d, %*d)\n", 
        dims[origin].name, mlen, ox, mlen, oz, dims[dest].name, mlen, dx, mlen, dz);
    
    return 0;
}