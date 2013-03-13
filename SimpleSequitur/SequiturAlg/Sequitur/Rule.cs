/*
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
*/

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SequiturAlg
{
    public class Rule : RuleSymbol
    {
        Sequitur _Algorithm;
        public Sequitur Algorithm { get { return _Algorithm; } }

        int _Count;
        public int Count { get { return _Count; } }

        String _ID;
        public String ID { get { return _ID; } }

        public void reuse()
        {
            _Count++;
        }

        public void deuse()
        {
            _Count--;
        }


        LinkedList<Symbol> _Symbols;

        public LinkedList<Symbol> Symbols 
        { 
            get 
            { 
                return _Symbols; 
            } 
        }

        public Rule(string ID, Sequitur algorithm)
        {
            _Algorithm = algorithm;
            this._ID = ID;
            _Symbols = new LinkedList<Symbol>();
        }

        internal RecursionPoint CheckTail()
        {

            if( Symbols.First.Equals(Symbols.Last) )
                return new RecursionPoint();
            var result = Algorithm.findOccurence(Symbols.Last.Previous);
            if ( result == OverlapInfo.NotFound )
            {
                Algorithm.noteDigram(Symbols.Last.Previous);
                return new RecursionPoint();
            }
            else if( result == OverlapInfo.Found)
            {            
                return Algorithm.replace(Symbols.Last.Previous);
            }
            return new RecursionPoint();
        }

        public bool IsTerminal
        {
            get { return false; }
        }

        public Rule Content { get { return this; } }

        public String Representation
        {
            get { return ID; }
        }

        public string EvaluatedString
        {
            get 
            {
                StringBuilder ss = new StringBuilder();
                foreach (var s in Content.Symbols)
                {
                    ss.Append(s.EvaluatedString);
                }
                return ss.ToString();
            }
        }

        public bool Equals(Symbol other)
        {
            return (other as Rule) == this;
        }
    }
}
