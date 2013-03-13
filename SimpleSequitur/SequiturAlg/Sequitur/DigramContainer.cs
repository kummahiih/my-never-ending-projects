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

    public class Digram: IEquatable<Digram>
    {
        public Symbol first;
        public Symbol second;

        public Digram(LinkedListNode<Symbol> ls)
        {
            Debug.Assert(ls != null /*&& ls.Next != null*/);
            if(ls != null)
                first = ls.Value;
            if (ls.Next != null) 
                second = ls.Next.Value;
        }

        public bool Equals(Digram other)
        {
            if (other == null)
                return false;
            if ((other.second != null) != (second != null))
                return false;

            if (second == null)
                return other.first.Equals(first);

            return other.first.Equals(first) && other.second.Equals(second);
        }

        public override int GetHashCode()
        {
            if (second == null)
                return first.GetHashCode();
            return first.GetHashCode() ^ second.GetHashCode();
        }

        public override String ToString()
        {
            var r = "";
            if (first != null)
                r += first.ToString();
            if(second != null)
                r += "," + second.ToString();
            return "("+r+")";
        }
    }

    public enum OverlapInfo { Overlaps, NotFound, Found };

    public class DigramContainer
    {
        Dictionary<Digram, DigramEntry> _FirstOccurances;
        public Dictionary<Digram, DigramEntry> FirstOccurances { get { return _FirstOccurances; } }

        Dictionary<LinkedList<Symbol>, DigramEntry> _RuleAnchors;
        public Dictionary<LinkedList<Symbol>, DigramEntry> RuleAnchors { get { return _RuleAnchors; } }

        int _NextId;
        public DigramContainer()
        {
            _NextId = 0;
            _FirstOccurances = new Dictionary<Digram, DigramEntry>();
            _RuleAnchors = new Dictionary<LinkedList<Symbol>, DigramEntry>();
        }

        public string NextId { 
            get 
            {
                _NextId++; 
                return _NextId.ToString(); 
            } 
        }

        

        internal OverlapInfo NonOverlappingOccurenceFound(LinkedListNode<Symbol> linkedListNode)
        {
            var digram = new Digram(linkedListNode);
            if(!FirstOccurances.ContainsKey(digram))
                return OverlapInfo.NotFound;

            var foundEntry = FirstOccurances[digram];

            if (this.RuleAnchors.ContainsKey(linkedListNode.List) && this.RuleAnchors[linkedListNode.List].Rule == foundEntry.Rule)
                return OverlapInfo.Overlaps;

            return foundEntry.Overlaps(linkedListNode) ? OverlapInfo.Overlaps : OverlapInfo.Found;
        }


        internal Rule getRule(LinkedListNode<Symbol> linkedListNode)
        {
            var digram = new Digram(linkedListNode);
            var foundEntry = FirstOccurances[digram];
            return foundEntry.Rule;
        }

        internal RecursionPoint newRule(LinkedListNode<Symbol> linkedListNode, Rule rule)
        {
            RecursionPoint todo = new RecursionPoint();

            var olddigram = new Digram(linkedListNode);
            var foundEntry = FirstOccurances[olddigram];

            var currentEntry = new DigramEntry(linkedListNode);

            var prev = foundEntry.StartPoint.Previous;
            if (prev != null )
                todo.RemovedDigrams.Add(new Digram(prev));

            var next = foundEntry.StartPoint.Next;
            if (next != null && next.Next != null)
                todo.RemovedDigrams.Add(new Digram(next));

            foundEntry.Rule = rule;
            RuleAnchors[rule.Symbols] = foundEntry;
            
            if (foundEntry.StartPoint.List.First.Next == foundEntry.StartPoint.List.Last &&
                this.RuleAnchors.ContainsKey(foundEntry.StartPoint.List))
            {
                Debug.Print("refreshin a substitute rule:" + foundEntry.ToString() + " -> " + this.RuleAnchors[foundEntry.StartPoint.List].ToString()); 
                this.FirstOccurances[new Digram(foundEntry.StartPoint)] = this.RuleAnchors[foundEntry.StartPoint.List];
            }
            else
            {
                todo.DigramsToCheck.Add(foundEntry.StartPoint.Previous);
                todo.DigramsToCheck.Add(foundEntry.StartPoint);
            }

            return todo;
        }

        internal RecursionPoint setRule(LinkedListNode<Symbol> linkedListNode, Rule rule)
        {
            RecursionPoint todo = new RecursionPoint();

            var adigramEntry = new DigramEntry(linkedListNode);
            var olddigram = new Digram(linkedListNode.List.First);

            {
                var prev = adigramEntry.StartPoint.Previous;
                if (prev != null)
                    todo.RemovedDigrams.Add(new Digram(prev));

                var next = adigramEntry.StartPoint.Next;
                if (next != null && next.Next != null)
                    todo.RemovedDigrams.Add(new Digram(next));
            }

            adigramEntry.Rule = rule;

            

            var symbolpntr = rule.Symbols.First;

            bool substituted = false;

            while(symbolpntr != null)
            {
                var symbol= symbolpntr.Value;
                if (!symbol.IsTerminal)
                {
                    Rule pointedRule = (symbol as RuleInstance).Content; 
                    pointedRule.deuse();
                    
                    if (pointedRule.Count <= 1)
                    {
                        Debug.Assert(pointedRule.ID != rule.ID);
                        Debug.Print("substituting a rule:" + pointedRule.ID);
                        Debug.Print("into a rule:" + rule.ID);

                        substituted = true;

                        todo.DigramsToCheck.Add(symbolpntr.Previous);

                        
                        var digram = new Digram(pointedRule.Symbols.First);
                        if (FirstOccurances.ContainsKey(digram))
                            FirstOccurances.Remove(digram);

                        if(RuleAnchors.ContainsKey(pointedRule.Symbols))
                            RuleAnchors.Remove(pointedRule.Symbols);

                        var movedsymbol = pointedRule.Symbols.First;
                        while(movedsymbol != null)
                        {
                            var next = movedsymbol.Next;                        
                            symbolpntr.List.AddBefore(symbolpntr, movedsymbol.Value);
                            todo.DigramsToCheck.Add(symbolpntr.Previous);
                            movedsymbol = next;
                        }
                        symbolpntr = symbolpntr.Previous;
                        rule.Symbols.Remove(symbolpntr.Next);
                        
                        
                    }                   
                }
                symbolpntr = symbolpntr.Next;
            }

            if (substituted)
            {
                Debug.Print("Removing old substitute rule "+ olddigram.ToString());
                this._FirstOccurances.Remove(olddigram);
                todo.DigramsToCheck.Add(adigramEntry.StartPoint.Previous);
                todo.DigramsToCheck.Add(adigramEntry.StartPoint);
            }
            else
            {
                if (adigramEntry.StartPoint.List.First.Next == adigramEntry.StartPoint.List.Last &&
                    this.RuleAnchors.ContainsKey(adigramEntry.StartPoint.List))
                {
                    Debug.Print("refreshin a substitute rule:" +adigramEntry.ToString() + " -> " +this.RuleAnchors[adigramEntry.StartPoint.List].ToString()); 
                    this.FirstOccurances[new Digram(adigramEntry.StartPoint)] = this.RuleAnchors[adigramEntry.StartPoint.List];
                }
                else
                {
                    todo.DigramsToCheck.Add(adigramEntry.StartPoint.Previous);
                    todo.DigramsToCheck.Add(adigramEntry.StartPoint);
                }
            }

            return todo;
        }

        internal void removeDigrams(HashSet<Digram> removedDigrams)
        {
            
            foreach (var digram in removedDigrams)
            {
                if (!FirstOccurances.ContainsKey(digram))
                    continue;
                //Debug.Assert(!FirstOccurances[digram].IsRule);
                if(!FirstOccurances[digram].IsRule)
                    FirstOccurances.Remove(digram);
            }
        }

        internal void noteDigram(LinkedListNode<Symbol> linkedListNode)
        {
            var digram = new Digram(linkedListNode);
            Debug.Print("New digram:" + digram.first.Representation + "," + digram.second.Representation);
            FirstOccurances[digram] = new DigramEntry(linkedListNode);
        }
    }
}
