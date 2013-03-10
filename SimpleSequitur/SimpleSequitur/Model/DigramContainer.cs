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

    public class Digram: IEquatable<Digram>
    {
        public Symbol first;
        public Symbol second;

        public Digram(LinkedListNode<Symbol> ls)
        {
            Debug.Assert(ls != null && ls.Next != null);
            if(ls != null)
                first = ls.Value;
            if (ls.Next != null) 
                second = ls.Next.Value;
        }

        public bool Equals(Digram other)
        {
            return other.first.Equals(first) && other.second.Equals(second);
        }

        public override int GetHashCode()
        {
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

        /*Tuple<Symbol, Symbol> Digram(LinkedListNode<Symbol> ls)
        {
            return new Tuple<Symbol, Symbol>(ls.Value, ls.Next.Value);
        }*/


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

            var digram = new Digram(linkedListNode);
            var foundEntry = FirstOccurances[digram];

            var currentEntry = new DigramEntry(linkedListNode);

            var prev = foundEntry.StartPoint.Previous;
            if (prev != null )
                todo.RemovedDigrams.Add(new Digram(prev));

            var next = foundEntry.StartPoint.Next;
            if (next != null && next.Next != null)
                todo.RemovedDigrams.Add(new Digram(next));

            foundEntry.Rule = rule;
            RuleAnchors[rule.Symbols] = foundEntry;



            //if (!refreshOccurances(foundEntry))
            {
                todo.DigramsToCheck.Add(foundEntry.StartPoint.Previous);
                todo.DigramsToCheck.Add(foundEntry.StartPoint);
            }

            return todo;
        }

        private bool refreshOccurances(DigramEntry foundEntry)
        {
            //if found entry in rule that should be in the FirstOccurances dictinary -> put it there
            if (foundEntry.StartPoint.List.First.Next == foundEntry.StartPoint.List.Last)
            {
                if (RuleAnchors.ContainsKey(foundEntry.StartPoint.List))
                {
                    var ancestorEntry = RuleAnchors[foundEntry.StartPoint.List];
                    Debug.Print("refresh ancestor entry" + ancestorEntry.ToString() );
                    FirstOccurances[new Digram(foundEntry.StartPoint.List.First)] = ancestorEntry;
                    return true;
                }
            }
            return false;
        }

        internal RecursionPoint setRule(LinkedListNode<Symbol> linkedListNode, Rule rule)
        {
            RecursionPoint todo = new RecursionPoint();

            var adigram = new DigramEntry(linkedListNode);
            
            {
                var prev = adigram.StartPoint.Previous;
                if (prev != null)
                    todo.RemovedDigrams.Add(new Digram(prev));

                var next = adigram.StartPoint.Next;
                if (next != null && next.Next != null)
                    todo.RemovedDigrams.Add(new Digram(next));
            }

            adigram.Rule = rule;

            //if (!refreshOccurances(adigram))
            {
                todo.DigramsToCheck.Add(adigram.StartPoint.Previous);
                todo.DigramsToCheck.Add(adigram.StartPoint);
            }

            var symbolpntr = rule.Symbols.First;

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

                        todo.DigramsToCheck.Add(symbolpntr.Previous);

                        //
                        //var digram = new Digram(pointedRule.Symbols.First);
                        //if (FirstOccurances.ContainsKey(digram))
                        //    FirstOccurances.Remove(digram);


                        var movedsymbol = pointedRule.Symbols.First;
                        while(movedsymbol != null)
                        {
                            var next = movedsymbol.Next;
                            //pointedRule.Symbols.Remove(movedsymbol);
                            symbolpntr.List.AddBefore(symbolpntr, movedsymbol.Value);
                            movedsymbol = next;
                        }
                        symbolpntr = symbolpntr.Previous;
                        todo.DigramsToCheck.Add(symbolpntr);
                        rule.Symbols.Remove(symbolpntr.Next);
                    }                   
                }
                symbolpntr = symbolpntr.Next;
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
