/*
 *       align.cpp
 *
 *       Copyright 2011 Vitor Onuchic <vitoronuchic@gmail.com>
 *                      Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonádio <ibonadio@ime.usp.br>
 *                      Alan Mitchell Durham <aland@usp.br>
 *
 *       This program is free software; you can redistribute it and/or modify
 *       it under the terms of the GNU  General Public License as published by
 *       the Free Software Foundation; either version 3 of the License, or
 *       (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public License
 *       along with this program; if not, write to the Free Software
 *       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *       MA 02110-1301, USA.
 */

#include <boost/program_options.hpp>
#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#include "ProbabilisticModel.hpp"
#include "PairHiddenMarkovModel.hpp"
#include "SequenceEntry.hpp"
#include "SequenceFormat.hpp"

#include "ProbabilisticModelCreatorClient.hpp"
#include "version.hpp"

using namespace tops;
using namespace std;
using namespace boost::program_options;

void alignSeqs(ProbabilisticModelPtr model, SequenceList seqs1, SequenceList seqs2, vector<std::string> names1, vector<std::string> names2, ostream & output)
{
  if(seqs1.size() != seqs2.size()){
    cerr << "Both files must to have the same amount of sequences." << endl;
    exit(-1);
  }
  for(int i = 0; i < (int)seqs1.size(); i++)
    {
      Sequence s1,s2;
      std::stringstream aux1,aux2;
      model->pairDecodable()->posteriorDecoding(seqs1[i], seqs2[i], s1, s2);
      aux1 << names1[i] << "_" << names2[i] << "_al1" ;
      aux2 << names1[i] << "_" << names2[i] << "_al2" ;
      SequenceEntry out1(model->alphabet());
      SequenceEntry out2(model->alphabet());
      out1.setName(aux1.str());
      out2.setName(aux2.str());
      out1.setSequence(s1);
      out2.setSequence(s2);
      output << out1;
      output << out2 << endl;
    }
}

int main (int argc, char ** argv)
{
    srand(time(NULL));

    options_description desc("Allowed options");
    desc.add_options()
      ("help,?", "produce help message")
      ("model,m", value<string> (), "model")
      ("output,o", value<string>(), "file to store  the sequences")
      ("asequence,a", value<string> (), "sequence1 file")
      ("bsequence,b", value<string> (), "sequence2 file")
      ("fasta,F",  "use fasta format");
    try
    {
        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);

        if(vm.count("help"))
        {
            cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
            cout << std::endl;
            cout << desc << "\n";
            return 1;
        }


        string model_file = vm["model"].as<string>();
        ProbabilisticModelCreatorClient creator;
        ProbabilisticModelPtr model = creator.create(model_file);
        if(model == NULL)
          {
            exit(-1);
          }

        if(vm.count("fasta") )
          SequenceFormatManager::instance()->setFormat(FastaSequenceFormatPtr(new FastaSequenceFormat()));

        SequenceEntryList sample_set1, sample_set2;
        AlphabetPtr alphabet = model->alphabet();
        readSequencesFromFile(sample_set1, alphabet, (vm["asequence"].as<string>()).c_str());
        readSequencesFromFile(sample_set2, alphabet, (vm["bsequence"].as<string>()).c_str());
        SequenceList seqs1, seqs2;
        vector<std::string> names1, names2;
        for(int i = 0; i < (int)sample_set1.size(); i++){
          seqs1.push_back(sample_set1[i]->getSequence());
          names1.push_back(sample_set1[i]->getName());
          seqs2.push_back(sample_set2[i]->getSequence());
          names2.push_back(sample_set2[i]->getName());
        }

        if(vm.count("output")) {
          string output = vm["output"].as<string>();
          ofstream obs_output(output.c_str());
	  alignSeqs(model, seqs1, seqs2, names1, names2, obs_output);
          obs_output.close();
        }
        else {
	  alignSeqs(model, seqs1, seqs2, names1, names2, std::cout);
        }

    }
    catch (boost::program_options::invalid_command_line_syntax &e)
    {
        cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
        cout << std::endl;

        cerr << "error: " << e.what() << std::endl;
        cerr << desc << endl;
    }
    catch (boost::program_options::unknown_option &e)
    {
        cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
        cout << std::endl;

        cerr << "error: " << e.what() << std::endl;
        cerr << desc << endl;
    }
    catch (boost::bad_any_cast & e)
    {
        cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
        cout << std::endl;
        cerr << desc << endl;
    }
    return 0;
}
