#include "PairHiddenMarkovModelCreator.hpp"
#include "MultinomialDistributionCreator.hpp"
#include "MultinomialDistribution.hpp"
#include "ConfigurationReader.hpp"
#include "PairHiddenMarkovModel.hpp"
#include "Alphabet.hpp"
#include "Symbol.hpp"

namespace tops {
  ProbabilisticModelPtr PairHiddenMarkovModelCreator::create(ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr state_names = parameters.getMandatoryParameterValue("state_names");
    ProbabilisticModelParameterValuePtr observation_symbols = parameters.getMandatoryParameterValue("observation_symbols");
    ProbabilisticModelParameterValuePtr number_of_emissions = parameters.getMandatoryParameterValue("number_of_emissions");
    ProbabilisticModelParameterValuePtr transitions = parameters.getMandatoryParameterValue("transitions");
    ProbabilisticModelParameterValuePtr emissions = parameters.getMandatoryParameterValue("emission_probabilities");

    if((state_names == NULL)||
       (observation_symbols == NULL)||
       (number_of_emissions == NULL) || 
       (transitions == NULL) || 
       (emissions == NULL)) 
      {
	std::cerr << help() << std::endl;
      }
    ProbabilisticModelPtr model = PairHiddenMarkovModelPtr(new PairHiddenMarkovModel());
    model->initialize(parameters);
    return model;
  }
}