[Skip straight to the English description](#english)

Au mois de juin 2017, Herb Sutter a publié une proposition d'évolution du langage : les métaclasses. La proposition comporte de nombreux exemples d'application du mécanisme qui enrichissent considérablement le langage. Non seulement cette proposition est-elle très emballante, elle est également accompagnée d'une version expérimentale de clang qui permet d'expérimenter concrètement avec ces idées. L'objectif de cet atelier est d'expérimenter avec cette proposition et d'échanger des idées d'applications pratiques. LES MÉTACLASSES NE SONT PAS UN STANDARD ET IL N'Y A PAS DE GARANTIE QU'ELLES LE SERONT SOUS CETTE FORME OU UNE AUTRE. À CE STADE, C'EST EXPÉRIMENTAL.

Il y a plusieurs étapes de préparation qui vous permettront de profiter au maximum de l'atelier : 
1. Compiler la version expérimentale de clang sur votre machine 
1. Lire cet article de Jonathan Boccara qui résume la proposition sur les métaclasses : http://www.fluentcpp.com/2017/08/04/metaclasses-cpp-summary/ 
1. Lire la proposition originale de Herb Sutter : http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0707r0.pdf 
1. Écouter la conférence de Herb Sutter à l'ACCU 2017 : https://youtu.be/6nsyX37nsRs 
1. Installer le Compiler Explorer de Matt Godbolt sur votre machine

Pour compiler clang, il faut d'abord télécharger plusieurs dépôts à des révisions spécifiques. Ensuite il faut suivre les instructions de LLVM pour assembler les projets de la bonne façon, invoquer CMake, bâtir et installer le tout. Voici les URLs des dépôts que vous devez télécharger (les autres éléments optionnels de LLVM ne sont pas requis) : 

1. LLVM : https://github.com/llvm-mirror/llvm/archive/40b1e969f9cb2a0c697e247435193fb006ef1311.zip 
1. clang : https://github.com/asutton/clang/archive/master.zip 
1. libcxx : https://github.com/llvm-mirror/libcxx/archive/64182a5877865cde2538c6038f98e3df33c93a03.zip 
1. libcxxabi : https://github.com/llvm-mirror/libcxxabi/archive/c515867bc14c433febcc574baedd081c078124d1.zip 
1. compiler-rt : https://github.com/llvm-mirror/compiler-rt/archive/06f1c090cb4e27ea82320bfa7af1e3cfb681edeb.zip 

Ensuite, il faut suivre les instructions d'installation de LLVM : https://github.com/llvm-mirror/llvm/blob/40b1e969f9cb2a0c697e247435193fb006ef1311/docs/GettingStarted.rst#getting-started-quickly-a-summary 

Finalement, si vous aimeriez utiliser le Compiler Explorer, il faut cloner l'URL suivant : https://github.com/mattgodbolt/compiler-explorer.git 

Pour l'installer, il faut d'abord installer Node.js, et ensuite il suffit d'invoquer `make`, ce qui va exécuter un serveur local avec la configuration par défaut. Ensuite, afin que le serveur offre le compilateur expérimental compilé un peu plus tôt, il faut modifier la configuration de la façon suivante : 
1- Ouvrir le fichier <compiler-explorer-path>/etc/config/c++.defaults.properties 
2- Modifier `compilers` afin d'y ajouter le chemin vers l'exécutable `clang++` que l'on vient de créer (c'est une liste de chemins séparés par le caractère ':') 
3- Modifier `defaultCompiler` afin d'utiliser le chemin du compilateur expérimental 
4- Modifier `options` afin d'y ajouter des paramètres de compilation par défaut : -std=c++1z -Xclang -freflection -I<clang-expérimental-path>/include -stdlib=libc++ -include cppx/meta -include cppx/compiler 
5- Ajouter, au besoin, un paramètre de compilation supplémentaire dans les `options`. Par exemple, sur mon Mac, j'ai dû ajouter le SDK de macOS avec le paramètre : -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk/usr/include 

-----

## English

In June 2017, Herb Sutter has submitted a proposal for a new language evolution feature : metaclasses. The proposal is filled with examples demonstrating the applications of this feature, considerably enriching the language. This proposal is not only very exciting on its own, it is also accompanied by an experimental version of the clang compiler which allows us to toy with this feature and experiment those ideas. The objective of the workshop is to experiment with metaclasses and share ideas amongst ourselves. METACLASSES AREN'T A STANDARD AND THERE IS NO GUARANTEE THEY'LL BECOME ONE UNDER THE CURRENT FORM OR ANOTHER. AT THIS POINT, IT IS ONLY EXPERIMENTAL. 

There are a few steps you should consider applying in order to prepare for the workshop : 
1- Compile the experimental compiler on your machine 
2- Read this article by Jonathan Boccara that summarizes the metaclasses proposal : http://www.fluentcpp.com/2017/08/04/metaclasses-cpp-summary/ 
3- Read the original proposal by Herb Sutter : http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0707r0.pdf 
4- Watch Herb Sutter's talk at the ACCU 2017 conference : https://youtu.be/6nsyX37nsRs 
5- Install Matt Godbolt's Compiler Explorer on your machine

In order to compile clang, you'll need to download a few repositories at specific commits where it is known to be working. Then you'll have to follow the instructions of LLVM to put everything together before you can invoke CMake and then build and install everything. Here are the URLs for the repositories you must download (the other optional elements of LLVM are not required) : 

1- LLVM : https://github.com/llvm-mirror/llvm/archive/40b1e969f9cb2a0c697e247435193fb006ef1311.zip 
2- clang : https://github.com/asutton/clang/archive/master.zip 
3- libcxx : https://github.com/llvm-mirror/libcxx/archive/64182a5877865cde2538c6038f98e3df33c93a03.zip 
4- libcxxabi : https://github.com/llvm-mirror/libcxxabi/archive/c515867bc14c433febcc574baedd081c078124d1.zip 
5- compiler-rt : https://github.com/llvm-mirror/compiler-rt/archive/06f1c090cb4e27ea82320bfa7af1e3cfb681edeb.zip 

Next, you must follow LLVM's installation instructions : https://github.com/llvm-mirror/llvm/blob/40b1e969f9cb2a0c697e247435193fb006ef1311/docs/GettingStarted.rst#getting-started-quickly-a-summary 

Finally, if you'd like to use the compiler explorer locally, you can clone this URL : https://github.com/mattgodbolt/compiler-explorer.git 

In order to install it, you must first install Node.js. Then you can invoke `make`, this is going to execute a local server with the default configuration. Next, we must change the configuration so that it picks up the experimental compiler. Here's how it must be configured : 
1- Open the file <compiler-explorer-path>/etc/config/c++.defaults.properties 
2- Modify `compilers` to add the path towards the `clang++` binary that we have compiled (paths are separated by the ':' character) 
3- Modify `defaultCompiler` to use the path to the experimental compiler 
4- Modify `options` to add default compilation options : -std=c++1z -Xclang -freflection -I<experimental-clang-path>/include -stdlib=libc++ -include cppx/meta -include cppx/compiler 
5- Add, if necessary, another compilation flag in the `options`. As an example, on my Mac, I had to add the macOS SDKusing the parameter : -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk/usr/include
