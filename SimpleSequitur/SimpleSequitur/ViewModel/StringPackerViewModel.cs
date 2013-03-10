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
using System.Windows.Input;

namespace SimpleSequitur.ViewModel
{
    public class StringPackerViewModel : ViewModelBase
    {
        public StringPackerViewModel()
        {
            _Input = "";
            _SplitFromSpaces = false;
            CreateTestCommand();
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

        bool _SplitFromSpaces;

        public bool SplitFromSpaces
        {
            get { return _SplitFromSpaces; }
            set 
            {
                if (value != _SplitFromSpaces)
                {
                    _SplitFromSpaces = value;
                    OnPropertyChanged("SplitFromSpaces");
                    OnPropertyChanged("Symbols");
                }
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
                    if (!SplitFromSpaces)
                    {
                        foreach (Char v in Input)
                        {
                            strings.Add(v.ToString());
                        }
                    }
                    else
                    {
                        strings = Input.Split(new char[4]{' ','\r','\n','\t'}).ToList();
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

        public ICommand TestCommand
        {
            get;
            internal set;
        }

        private bool CanExecuteTestCommand()
        {
            return true;
        }

        private void CreateTestCommand()
        {
            TestCommand = new RelayCommand( param => TestExecute(), param => CanExecuteTestCommand());
        }

        public void TestExecute()
        {
            int size  = 100;
            StringBuilder builder = new StringBuilder();
            Random random = new Random();
            char ch ;
            for(int i=0; i<size; i++)
            {
            ch = Convert.ToChar(Convert.ToInt32(Math.Floor(26 * random.NextDouble() + 65))) ;
            builder.Append(ch); 
            }
            Input = builder.ToString();
        }

    }
}
