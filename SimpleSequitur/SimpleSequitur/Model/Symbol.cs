/*
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
*/


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleSequitur
{
    public interface Symbol: IEquatable<Symbol>
    {
        bool IsTerminal { get; }
        String Representation { get; }
    }

    public class TerminalInstance : Symbol, IEquatable<Symbol>
    {
        String _Content;
        public String Content { get { return _Content; } }

        public TerminalInstance(String content)
        {
            _Content = content;
        }

        public bool IsTerminal
        {
            get { return true; }
        }

        public string Representation
        {
            get { return _Content; }
        }

        public bool Equals(Symbol other)
        {
            return other.IsTerminal && this.Content.Equals((other as TerminalInstance).Content);
        }

        public override int GetHashCode()
        {
            return _Content.GetHashCode();
        }

        public override string ToString()
        {
            return "'"+_Content+"'";
        }
    }

    public class RuleInstance : Symbol, IEquatable<Symbol>
    {
        Rule _Content;
        public Rule Content { get { return _Content; } }

        public RuleInstance(Rule content)
        {
            _Content = content;
            Content.reuse();
        }

        public override string ToString()
        {
            return Representation;
        }

        public bool IsTerminal
        {
            get { return false; }
        }

        public string Representation
        {
            get { return "R:" + _Content.ID; }
        }

        public bool Equals(Symbol other)
        {
            return (!other.IsTerminal) && Content.Equals( (other as RuleInstance).Content);
        }

        public override int GetHashCode()
        {
            return _Content.GetHashCode();
        }
    }
}
