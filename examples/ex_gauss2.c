#include <stdio.h>
#include <time.h>

#include <stc/crandom.h>
#include <stc/csmap.h>
#include <stc/cstr.h>

// Declare int -> int sorted map. Uses typetag 'i' for ints.
using_csmap(i, int, size_t);

int main()
{
    enum {N = 10000000};
    const double Mean = -12.0, StdDev = 6.0, Scale = 74;

    printf("Demo of gaussian / normal distribution of %d random samples\n", N);

    // Setup random engine with normal distribution.
    uint64_t seed = time(NULL);
    stc64_t rng = stc64_init(seed);
    stc64_normalf_t dist = stc64_normalf_init(Mean, StdDev);

    // Create histogram map
    csmap_i mhist = csmap_i_init();
    c_forrange (N) {
        int index = (int) round( stc64_normalf(&rng, &dist) );
        ++ csmap_i_emplace(&mhist, index, 0).first->second;
    }

    // Print the gaussian bar chart
    cstr bar = cstr_init();
    c_foreach (i, csmap_i, mhist) {
        size_t n = (size_t) (i.ref->second * StdDev * Scale * 2.5 / N);
        if (n > 0) {
            cstr_resize(&bar, n, '*');
            printf("%4d %s\n", i.ref->first, bar.str);
        }
    }
    // Cleanup
    cstr_del(&bar);
    csmap_i_del(&mhist);
}
