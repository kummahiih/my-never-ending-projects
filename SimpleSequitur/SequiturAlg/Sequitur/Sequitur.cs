/*
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
*/


using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SequiturAlg
{
    public class Sequitur
    {
        Rule _StartRule;
        public RuleInstance StartRule { get { return new RuleInstance(_StartRule); } }

        DigramContainer _Data;
        public DigramContainer Data { get { return _Data; } }
        public HashSet<Rule> Rules { get; protected set; }

        public Sequitur()
        {
            Rules = new HashSet<Rule>();
            _Data = new DigramContainer();
            _StartRule = new Rule("startrule", this);
            //Rules.Add(_StartRule);
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

                    Data.removeDigrams(todo.RemovedDigrams);
                    todo.RemovedDigrams.Clear();
                    
                    todo.UnionWith(_StartRule.CheckTail());
                }
            }
        }

        public HashSet<Rule> getRules()
        {
            //no hashset...
            ConcurrentDictionary<Rule, int> returned = new ConcurrentDictionary<Rule, int>();

            //ConcurrentBag<Action> tasks = new ConcurrentBag<Action>();

            //just studying how to use tasks
            returned[StartRule.Content] = 1;

            Action<Rule> rules = null;
            rules = (Rule r) =>
                 {
                     List<Task> tasks = new List<Task>();
                     r.Content.Symbols.ToList().ForEach(r2 =>
                         {
                             if (!r2.IsTerminal)
                             {
                                 tasks.Add( Task.Factory.StartNew(() =>
                                     {
                                         returned.AddOrUpdate((r2 as RuleInstance).Content,0, (Rule r3, int cnt) => cnt+1);
                                         rules((r2 as RuleInstance).Content);
                                     }));
                             }
                         }
                     );
                     Task.WaitAll(tasks.ToArray());
                 };
            rules(StartRule.Content);

            return new HashSet<Rule>(returned.Keys);
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
#if DEBUG
                Rules.Add(rule);
#endif

                Debug.Print("New rule created:" + rule.ID);

                todo.UnionWith(Data.newRule(linkedListNode, rule));
            }
            else
            {
                Debug.Print("using old rule:" + rule.ID );
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
