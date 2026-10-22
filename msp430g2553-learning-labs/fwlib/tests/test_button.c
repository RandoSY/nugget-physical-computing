#include <assert.h>
#include "edu430_button.h"
int main(void)
{
 edu_button_t b={0u,0u,0u};
 assert(edu_button_pressed(&b,1u,3u)==0u);
 assert(edu_button_pressed(&b,0u,3u)==0u); /* bounce */
 assert(edu_button_pressed(&b,1u,3u)==0u);
 assert(edu_button_pressed(&b,1u,3u)==0u);
 assert(edu_button_pressed(&b,1u,3u)==1u);
 assert(edu_button_pressed(&b,1u,3u)==0u); /* held */
 edu_button_reset(&b);
 assert(edu_button_pressed(&b,1u,1u)==1u);
 return 0;
}
