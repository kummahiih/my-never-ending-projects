/*
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SimpleSequitur.ViewModel
{
    public class SymbolViewModel: ViewModelBase
    {
        Symbol Symbol { get; set; }
        public SymbolViewModel(Symbol s)
        {
            Symbol = s;
        }

        public String Name
        {
            get
            {
                String name = Symbol.Representation; 
                if (!Symbol.IsTerminal)
                    name += " C:"+(Symbol as RuleInstance).Content.Count.ToString();
                return name;
            }
        }

        public List<SymbolViewModel> Symbols
        {
            get
            {
                List<SymbolViewModel> returned = new List<SymbolViewModel>();
                if (Symbol.IsTerminal)
                    return returned;
                else
                {
                    foreach (Symbol s in (Symbol as RuleInstance).Content.Symbols)
                    {
                        returned.Add(new SymbolViewModel(s));
                    }
                    return returned;
                }
            }
        }

        public bool IsSelected { get; set; }
        public bool IsExpanded { get; set; }


    }
}
