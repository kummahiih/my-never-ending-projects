﻿/*
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
            first = ls.Value;
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
            return first.ToString() + "," + second.ToString();
        }
    }

    public class DigramContainer
    {
        Dictionary<Digram, DigramEntry> _FirstOccurances;
        public Dictionary<Digram, DigramEntry> FirstOccurances { get { return _FirstOccurances; } }

        int _NextId;
        public DigramContainer()
        {
            _NextId = 0;
            _FirstOccurances = new Dictionary<Digram, DigramEntry>();
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

        internal bool NonOverlappingOccurenceFound(LinkedListNode<Symbol> linkedListNode)
        {
            var digram = new Digram(linkedListNode);
            if(!FirstOccurances.ContainsKey(digram))
                return false;

            var foundEntry = FirstOccurances[digram];

            return !foundEntry.Overlaps(linkedListNode);
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


            todo.DigramsToCheck.Add(foundEntry.StartPoint.Previous);
            todo.DigramsToCheck.Add(foundEntry.StartPoint);

            return todo;
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

            todo.DigramsToCheck.Add(adigram.StartPoint.Previous);
            todo.DigramsToCheck.Add(adigram.StartPoint);


            var symbolpntr = rule.Symbols.First;

            while(symbolpntr != null)
            {
                var symbol= symbolpntr.Value;
                if (!symbol.IsTerminal)
                {
                    Rule pointedRule = (symbol as RuleInstance).Content; 
                    pointedRule.deuse();
                    /*
                     * TODO: make digrams to point optionaly more than to 2 symbols :) 
                     * if (pointedRule.Count <= 1)
                    {
                        Debug.Print("substituting a rule:" + pointedRule.ID);
                        Debug.Print("into a rule:" + rule.ID);

                        //todo.DigramsToCheck.Add(symbolpntr.Previous);

                        var digram = new Digram(pointedRule.Symbols.First);
                        if (FirstOccurances.ContainsKey(digram))
                            FirstOccurances.Remove(digram);


                        var movedsymbol = pointedRule.Symbols.First;
                        while(movedsymbol != null)
                        {
                            var next = movedsymbol.Next;
                            pointedRule.Symbols.Remove(movedsymbol);
                            symbolpntr.List.AddBefore(symbolpntr, movedsymbol);
                            movedsymbol = next;
                        }
                        symbolpntr = symbolpntr.Previous;
                        //todo.DigramsToCheck.Add(symbolpntr);
                        rule.Symbols.Remove(symbolpntr.Next);
                    } */                  
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
                Debug.Assert(!FirstOccurances[digram].IsRule);
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