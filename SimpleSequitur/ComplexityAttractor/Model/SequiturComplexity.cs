using SequiturAlg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ComplexityAttractor.Model
{
    public class SequiturComplexity
    {
        String chars;
        public String S {get;protected set;}
        public SequiturComplexity(String s)
        {
            S = s;
            chars =  "qwertyuiopåasdfghjklöäzxcvbnm";
            chars += chars.ToUpper();
            chars += "1234567890+-.,<>|;:!?\"()";
        }

        char getRandomChar()
        {
            return chars[new Random().Next(0, chars.Length)];
        }

        public int complexity(String s)
        {
            Sequitur alg = new Sequitur();
            List<String> strings = new List<string>();
            s.ToList().ForEach(c => strings.Add(c.ToString()));
            alg.Evaluate(strings);
            HashSet<Rule> rl = alg.getRules();
            int symbol_leght = 1; // start rule
            rl.ToList().ForEach(r => symbol_leght += r.Symbols.Count);
            return symbol_leght;
        }

        public String FirstMutation
        {
            get
            {
                int bestCompl = complexity(S);
                String best = S;
                foreach (var m in MutationsSwap())
                {
                    var c = complexity(m);
                    if (c >= bestCompl)
                        continue;
                    best = m;
                    bestCompl = c;
                }
                return best;
            }
        }

        IEnumerable<String> MutationsChar()
        {
            for (int i = 0; i < S.Length; i++)
            {
                foreach (var ch in chars)
                {
                    yield return S.Substring(0, i) + ch.ToString() + S.Substring(i + 1);
                }
            }
        }

        IEnumerable<String> MutationsSwap()
        {
            for (int i = 0; i < S.Length-1; i++)
            {
                for (int j = i+1; j < S.Length; j++)
                {

                    var s2 = S.Substring(0, i) + S[j].ToString() + S.Substring(i + 1, j -i -1) + S[i].ToString() + S.Substring(j);

                    yield return s2;
                }
                
            }
        }


    }
}
