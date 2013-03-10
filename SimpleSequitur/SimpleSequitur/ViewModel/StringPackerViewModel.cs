/*
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
*/


using SimpleSequitur.Model;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SimpleSequitur.ViewModel
{
    public class StringPackerViewModel : ViewModelBase
    {
        public StringPackerViewModel()
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
                    OnPropertyChanged("Symbols");
                }
            }
            get
            {
                return _Input;
            }
        }

        Sequitur _Sequitur;

        public List<SymbolViewModel> Symbols
        {
            get
            {
                List<SymbolViewModel> returned = new List<SymbolViewModel>();
                try
                {
                    Sequitur test = new Sequitur();

                    List<String> strings = new List<string>();

                    foreach (Char v in Input)
                    {
                        strings.Add(v.ToString());
                    }
                    test.Evaluate(strings);

                    
                    SymbolViewModel root = new SymbolViewModel( test.StartRule  );
                    _Sequitur = test;

                    returned.Add(root);
                    OnPropertyChanged("DigramEntries");
                }
                catch (Exception e)
                {
                    Debug.Print(e.StackTrace);
                }
                return returned;

            }
        }

        public List<String> DigramEntries
        {
            get
            {
                var entries = new List<String>();

                foreach (var e in _Sequitur.Data.FirstOccurances.AsEnumerable())
                {
                    entries.Add(e.Key.ToString() + "->" + e.Value.ToString());

                }
                return entries;
            }

        }

    }
}
