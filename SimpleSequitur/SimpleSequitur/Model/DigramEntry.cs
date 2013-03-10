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

namespace SimpleSequitur.Model
{
    public class DigramEntry
    {
        LinkedListNode<Symbol> _StartPoint;
        public LinkedListNode<Symbol> StartPoint { get { return _StartPoint; } }

        bool _IsRule;
        public bool IsRule { get { return _IsRule; } }

        public DigramEntry(LinkedListNode<Symbol> startpoint)
        {
            Debug.Assert(startpoint != null && startpoint.Next != null);
            _StartPoint = startpoint;
            _IsRule = false;
        }

        public bool Overlaps(LinkedListNode<Symbol> other)
        {
            if (IsRule)
                return false;

            if (StartPoint != other &&
                StartPoint.Next != other &&
                StartPoint != other.Next &&
                StartPoint.Next != other.Next)
                return false;

            return true;
        }

        public override string ToString()
        {
            if (IsRule)
                return (StartPoint.Value as RuleInstance).ToString();
            return new Digram(StartPoint).ToString();
        }

        public Rule Rule
        {
            get
            {
                if (IsRule)
                    return (StartPoint.Value as RuleInstance).Content;
                return null;
            }
            set
            {
                Debug.Assert(!IsRule);
                _IsRule = true;
                Debug.Print("setting rule:'" + value.ID + "':" + StartPoint.Value.Representation + "," + StartPoint.Next.Value.Representation);
                if (value.Symbols.First == null)
                {
                    value.Symbols.AddLast(StartPoint.Value);
                    value.Symbols.AddLast(StartPoint.Next.Value);
                }
                StartPoint.Value = new RuleInstance(value);
                StartPoint.List.Remove(StartPoint.Next);
            }
        }

    }
}
