#include "swarmalgorithmssettings.h"
#include "circlealgorithm.h"
#include "halfcirclealgorithm.h"
#include "linealgorithm.h"
#include "moveshapealgorithm.h"
SwarmAlgorithmsSettings swarmAlgorithmsSettings;

SwarmAlgorithmsSettings::SwarmAlgorithmsSettings()
{
   activeAlgorithms.append(new HalfCircleAlgorithm());
}
