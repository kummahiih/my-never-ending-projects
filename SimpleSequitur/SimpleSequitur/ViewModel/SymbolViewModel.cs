/*
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SequiturAlg;

namespace SimpleSequitur.ViewModel
{
    public class SymbolViewModel: ViewModelBase
    {
        public delegate void OnSymbolVMSelectedDelegate(SymbolViewModel s);
        OnSymbolVMSelectedDelegate _OnSymbolVMSelected;

        Symbol Symbol { get; set; }
        public SymbolViewModel(Symbol s, OnSymbolVMSelectedDelegate onSymbolSelected)
        {
            Symbol = s;
            _IsSelected = false;
            _OnSymbolVMSelected = onSymbolSelected;
            _Symbols = new List<SymbolViewModel>();
        }

        public String Name
        {
            get
            {
                if (Symbol == null)
                    return "";
                return  Symbol.Representation;
            }
        }
        List<SymbolViewModel> _Symbols;
        public List<SymbolViewModel> Symbols
        {
            get
            {

                if (Symbol == null || Symbol.IsTerminal)
                    return _Symbols;
                else
                {
                    _Symbols.Clear();
                    foreach (Symbol s in (Symbol as RuleSymbol).Content.Symbols)
                    {
                        _Symbols.Add(new SymbolViewModel(s, _OnSymbolVMSelected));
                    }
                    return _Symbols;
                }
            }
        }

        public String SymbolsAsString
        {
            get
            {
                if (Symbol == null)
                    return "";
                if(Symbol.IsTerminal)
                    return (Symbol as TerminalInstance).Content;

                StringBuilder ss = new StringBuilder();
                foreach (Symbol s in (Symbol as RuleSymbol).Content.Symbols)
                {
                    ss.AppendFormat(" {0},", s.Representation);
                }
                return ss.ToString().TrimEnd(',');
            }
        }

        public String UseCount
        {
            get
            {
                if (Symbol == null)
                    return "";
                if (Symbol.IsTerminal)
                    return "-";
                return (Symbol as RuleSymbol).Content.Count.ToString();
            }
        }

        bool _IsSelected;
        public bool IsSelected
        {
            get { return _IsSelected; }
            set
            {
                if (_IsSelected != value)
                {
                    _IsSelected = value;
                    if (_OnSymbolVMSelected != null)
                        _OnSymbolVMSelected(this);
                    OnPropertyChanged("IsSelected");
                }
            }
        }
        public bool IsExpanded { get; set; }

        public String EvaluatedString { get {
            if (Symbol == null)
                return "";
            return this.Symbol.EvaluatedString; } }


    }
}
