/*
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SequiturAlg
{
    public class RecursionPoint
    {
        HashSet<Digram> _RemovedDigrams;
        public HashSet<Digram> RemovedDigrams { get { return _RemovedDigrams; } }

        HashSet<LinkedListNode<Symbol>> _DigramsToCheck;
        public HashSet<LinkedListNode<Symbol>> DigramsToCheck { get { return _DigramsToCheck; } }



        public RecursionPoint()
        {
            _RemovedDigrams = new HashSet<Digram>();
            _DigramsToCheck = new HashSet<LinkedListNode<Symbol>>();
        }

        internal void UnionWith(RecursionPoint recursionPoint)
        {
            RemovedDigrams.UnionWith(recursionPoint.RemovedDigrams);
            DigramsToCheck.UnionWith(recursionPoint.DigramsToCheck);
        }

        public bool Empty { get { return RemovedDigrams.Count == 0 && DigramsToCheck.Count == 0; } }
    }
}
