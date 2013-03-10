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
    public class Sequitur
    {
        Rule _StartRule;
        public RuleInstance StartRule { get { return new RuleInstance(_StartRule); } }

        DigramContainer _Data;
        public DigramContainer Data { get { return _Data; } }

        public Sequitur()
        {
            _Data = new DigramContainer();
            _StartRule = new Rule(_Data.NextId, this);
            _StartRule.reuse();
        }

        public void Evaluate(List<String> items)
        {
            foreach (var item in items)
            {
                Debug.Print("read:'" + item + "'");
                _StartRule.Symbols.AddLast(new TerminalInstance(item));                
                RecursionPoint todo = _StartRule.CheckTail();
                while (todo != null && !todo.Empty )
                {
                    while(todo.DigramsToCheck.Count > 1)
                    {

                        RecursionPoint newtodo = new RecursionPoint();

                        foreach (var tocheck in todo.DigramsToCheck)
                        {
                            if (tocheck == null)
                                continue;

                            OverlapInfo result = findOccurence(tocheck) ;
                            if (result == OverlapInfo.Found)
                            {
                                newtodo.UnionWith(replace(tocheck));
                            }
                            else if( result == OverlapInfo.NotFound)
                            {
                                noteDigram(tocheck);
                            }
                            //otherwise overlaps
                        }
                        todo = newtodo;
                    }

                    foreach (var toexpand in todo.RulesToSkip)
                    {
                        Debug.Assert(Data.FirstOccurances.ContainsKey(toexpand));
                        if (Data.FirstOccurances.ContainsKey(toexpand))
                        {
                            var oldEntry = Data.FirstOccurances[toexpand];
                            Digram newDigram = new Digram(oldEntry.StartPoint);
                            Debug.Assert(Data.FirstOccurances.ContainsKey(newDigram));
                            var newEntry = Data.FirstOccurances[newDigram];
                            Data.FirstOccurances[toexpand] = newEntry;
                            //now the old rule can not be applied again by using that entry
                            //we could remove it here .. and thats what we do
                            if (newEntry.Rule != null)
                                oldEntry.swapRule(newEntry.Rule);
                        }
                        else
                        {
                            //it has not been added to the dictionary at all?!
                        }
                    }
                    todo.RulesToSkip.Clear();

                    //useless?
                    Data.removeDigrams(todo.RemovedDigrams);
                    todo.RemovedDigrams.Clear();
                    
                    todo.UnionWith(_StartRule.CheckTail());
                }
            }
        }


        internal OverlapInfo findOccurence(LinkedListNode<Symbol> linkedListNode)
        {
            if (linkedListNode == null || linkedListNode.Next == null)
                return OverlapInfo.NotFound;

            return Data.NonOverlappingOccurenceFound(linkedListNode);
        }


        internal RecursionPoint replace(LinkedListNode<Symbol> linkedListNode)
        {
            var rule = Data.getRule(linkedListNode);
            RecursionPoint todo = new RecursionPoint();
            var pointsToCheck = new HashSet<LinkedListNode<Symbol>>();
            if (rule == null)
            {
                rule = new Rule(Data.NextId, this);

                Debug.Print("New rule created:" + rule.ID);

                todo.UnionWith(Data.newRule(linkedListNode, rule));
            }
            else
            {
                Debug.Print("using old rule:" + rule.ID);
            }

            todo.UnionWith(Data.setRule(linkedListNode, rule));
            
            

            Data.removeDigrams(todo.RemovedDigrams);
            todo.RemovedDigrams.Clear();


            return todo;
        }

        internal void noteDigram(LinkedListNode<Symbol> linkedListNode)
        {
            if (linkedListNode == null || linkedListNode.Next == null)
                return;


            Data.noteDigram(linkedListNode);
        }
    }
}
