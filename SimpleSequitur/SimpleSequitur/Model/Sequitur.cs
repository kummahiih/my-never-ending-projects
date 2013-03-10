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
                while (todo != null && !todo.Empty)
                {
                    RecursionPoint newtodo = new RecursionPoint();
                    HashSet<LinkedListNode<Symbol>> overlapped = new HashSet<LinkedListNode<Symbol>>();

                    foreach (var tocheck in todo.DigramsToCheck)
                    {
                        if (tocheck == null)
                            continue;

                        if (overlapped.Contains(tocheck))
                        {
                            //not valid anymore.
                            continue;
                        }
                        if (ShouldReplace(tocheck))
                        {
                            overlapped.Add(tocheck.Next);
                            overlapped.Add(tocheck.Previous);
                            newtodo.UnionWith(replace(tocheck));
                        }
                        else
                        {
                            noteDigram(tocheck);
                        }
                    }
                    todo = newtodo;
                }
            }
        }


        internal bool ShouldReplace(LinkedListNode<Symbol> linkedListNode)
        {
            if (linkedListNode == null || linkedListNode.Next == null)
                return false;

            if (linkedListNode.List.First.Next == linkedListNode.List.Last)
            {
                Debug.Print("rejected replace:" + new Digram(linkedListNode).ToString() );
                return false;
            }

            if (Data.NonOverlappingOccurenceFound(linkedListNode))
                return true;
            return false;
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
            if (linkedListNode.List.First.Next == linkedListNode.List.Last)
            {
                Debug.Print("rejected Digram:" + new Digram(linkedListNode).ToString());
                return;
            }
            Data.noteDigram(linkedListNode);
        }
    }
}
