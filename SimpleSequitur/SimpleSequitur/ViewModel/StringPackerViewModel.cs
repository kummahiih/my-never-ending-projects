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
            CreateCommands();
            _Sequitur = new Sequitur();
            _SelectedSymbolVM = new SymbolViewModel(new TerminalInstance(""), null);
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

        SymbolViewModel _SelectedSymbolVM;
        public SymbolViewModel SelectedSymbolVM 
        {
            get
            {
                return _SelectedSymbolVM;
            }
            set
            {
                if (value != _SelectedSymbolVM)
                {
                    _SelectedSymbolVM = value;
                    OnPropertyChanged("SelectedSymbolVM");
                }
            }
        }

        public List<SymbolViewModel> Symbols
        {
            get
            {
                var returned = new  List<SymbolViewModel>();
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

                    SelectedSymbolVM = new SymbolViewModel(null, null);
                    returned.Add( new SymbolViewModel(test.StartRule, (SymbolViewModel s) => this.SelectedSymbolVM = s ));
                    foreach (var r in test.Rules)
                    {
                        returned.Add(new SymbolViewModel(r, (SymbolViewModel s) => this.SelectedSymbolVM = s));
                    }

                    _Sequitur = test;

                    Debug.Print("tree set (UI)");
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


        public ICommand RulesCommand
        {
            get;
            internal set;
        }

        private bool CanExecuteRulesCommand()
        {
            return true;
        }


        private void CreateCommands()
        {
            TestCommand = new RelayCommand( param => TestExecute(), param => CanExecuteTestCommand());
            RulesCommand = new RelayCommand(param => GetRules(), param => CanExecuteRulesCommand());
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

        public void GetRules()
        {
            List<Rule> r = _Sequitur.getRules();
            Debug.Print(r.Count.ToString());

            return;
        }

    }
}
