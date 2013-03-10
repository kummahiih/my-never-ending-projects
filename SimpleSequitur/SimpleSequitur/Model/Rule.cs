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

namespace SimpleSequitur
{
    public class Rule
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
                return null;

            if (!Algorithm.ShouldReplace(Symbols.Last.Previous))
            {
                Algorithm.noteDigram(Symbols.Last.Previous);
                return null;
            }
            
            return Algorithm.replace(Symbols.Last.Previous);
        }
    }
}
