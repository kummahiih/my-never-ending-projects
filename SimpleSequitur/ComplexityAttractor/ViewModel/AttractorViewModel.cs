using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SequiturAlg;
using ComplexityAttractor.Model;


namespace ComplexityAttractor.ViewModel
{
    public class AttractorViewModel: ViewModelBase
    {
        public AttractorViewModel()
        {
            _Input = "";
        }

        String _Input;
        public String Input
        {
            set
            {
                if (_Input != value)
                {
                    _Input = value;
                    OnPropertyChanged("Input");
                    OnPropertyChanged("OutStingList");
                    OnPropertyChanged("OutString");
                }
            }
            get
            {
                return _Input;
            }
        }


        public String OutString
        {
            get
            {
                StringBuilder sb = new StringBuilder();
                OutStingList.ForEach(s => sb.Append(s + "\n"));

                return sb.ToString();
            }

        }


        public List<String> OutStingList
        {
            get
            {
                List<String> returned = new List<string>();

                if (Input == "")
                    return returned;
                
                String iteration = Input;
                do
                {
                    returned.Add(iteration);
                    iteration = new SequiturComplexity(returned.Last()).FirstMutation;

                } while (iteration != returned.Last());
                

                return returned;
            }
        }
    }
}
