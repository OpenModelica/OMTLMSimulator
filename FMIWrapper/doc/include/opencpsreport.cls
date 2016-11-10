%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% opencpsreport.cls
% Template for INTO-CPS Deliverable Report
% Bernhard Thiele, 2016
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\NeedsTeXFormat{LaTeX2e}\relax
\ProvidesClass{include/opencpsreport}[2016/10/17 OPEN-CPS Report Template]


%%%% Load class article and redefine \maketitle after
%%%% declaring all the options
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{article}}
\ProcessOptions\relax
\LoadClass{article}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%% Requiring Packages
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\RequirePackage[T1]{fontenc}         %% better font encoding for accented characters (https://en.wikibooks.org/wiki/LaTeX/Fonts)
\RequirePackage{lmodern}             %% use computer Modern-like fonts with T1 support
\RequirePackage{mathptmx}            %% use fitting times fonts also in formulas
\RequirePackage{courier}             %% for proper courier fonts in texttt
% \RequirePackage[scaled=0.95]{helvet} %% scale Helvetica, so that its size matches better with the default font
\RequirePackage{hyperref}            %% display URLs
\RequirePackage{graphicx}            %% use graphics
\RequirePackage[RGB,table]{xcolor}   %% Require color
\RequirePackage{fancyhdr}            %% require fancy header
\RequirePackage{lastpage}            %% get \pageref*{LastPage} for footer line
\RequirePackage{tabularx}            %% adds possibility to maximize the width of columns (and enables word wrap)
\RequirePackage{multirow}            %% support multi row columns
\RequirePackage{colortbl}            %% better support for tables with colored elements
\RequirePackage{hhline}              %% get \hhline as a substitute to \cline which has the deficit that lines get covered by colour panels

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%% Various variables
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\newcommand{\opencps@reportnumber}{Dx.y}
\newcommand{\reportnumber}[1]{\renewcommand{\opencps@reportnumber}{#1}}
\newcommand{\opencps@reporttitle}{Deliverable Title}
\newcommand{\reporttitle}[1]{\renewcommand{\opencps@reporttitle}{#1}}
\newcommand{\opencps@shortreporttitle}{Short Deliverable Title}
\newcommand{\shortreporttitle}[1]{\renewcommand{\opencps@shortreporttitle}{#1}}
\newcommand{\opencps@reportaccess}{PU, CO}
\newcommand{\reportaccess}[1]{\renewcommand{\opencps@reportaccess}{#1}}
\newcommand{\opencps@deliverabletype}{Report, SW, Prototype}
\newcommand{\deliverabletype}[1]{\renewcommand{\opencps@deliverabletype}{#1}}
\newcommand{\opencps@duedate}{M12, M24, M36}

\newcommand{\version}[1]{\renewcommand{\opencps@version}{#1}}
\newcommand{\opencps@version}{1.0}

\newcommand{\duedate}[1]{\renewcommand{\opencps@duedate}{#1}}
\newcommand{\opencps@executivesummary}{Provide short summary/abstract here.}
\newcommand{\executivesummary}[1]{\renewcommand{\opencps@executivesummary}{#1}}



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%% Style settings
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\renewcommand{\sfdefault}{phv}                    %% Set sans serif font to Helvetica (https://en.wikibooks.org/wiki/LaTeX/Fonts)
\hypersetup{colorlinks=true, linkcolor=blue}      %% Set hyperref behaviour
\usepackage[left=1.0in, right=1.0in, top=0.8in, bottom=1.3in]{geometry} %% Set up basic geometry
\RequirePackage[parfill]{parskip}                 %% Make paragraphs start on new line, with no indentation
\setcounter{tocdepth}{3}                          %% Set depth of table of contents


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%% Document Header
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\pagestyle{fancy}
\lhead{\includegraphics[height=1.2cm]{include/opencps-logo-horiz}} % opencps logo
%Right side of header - include doc number, short title
\rhead{\footnotesize\opencps@reportnumber\ -\ \opencps@shortreporttitle
      \vspace{0.1cm}}
\setlength{\headheight}{15pt}
\renewcommand{\headrulewidth}{0.0pt} % no line
\lfoot{\footnotesize OPENCPS, ITEA3 Project no. 14018}
\cfoot{}
\rfoot{\footnotesize Page \thepage\ of {\pageref*{LastPage}}}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%% Title page
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\fancypagestyle{fancytitle}{%
\lhead{\footnotesize \sffamily OPENCPS\\
                ITEA3 Project no. 14018
                \vspace{0.2cm}}
\rhead{\includegraphics[height=1.5cm]{include/itea3-logo}} %opencps logo
%\setlength{\headheight}{40pt}
\setlength{\headheight}{40pt}
\renewcommand{\headrulewidth}{0.0pt} % no line
\lfoot{}
\cfoot{}
\rfoot{}
}


%% Change 'titlepage' into page with deliverable information
\def\maketitle{
\createdeliverableinfo
}

%% Deliverable information
\def\createdeliverableinfo{
 \thispagestyle{fancytitle}
 \renewcommand{\arraystretch}{1.4} % increase space between rows
 \vspace*{-2mm} % _Increase_ space to headline a bit, which seems weird, but deleting this line will actually decrease the vertical space
 \begin{center}
  \hypersetup{hidelinks}  % don't color footnote marks to match Word template
  \footnotesize\sffamily  % mimic Arial font used in Word template
	\begin{tabularx}{\linewidth}[h!]{|p{3cm}|X|}
	\hline
	\textbf{\LARGE\opencps@reportnumber}\rule{0pt}{0.8cm} & % get more vertical distance to upper \hline by invisble \rule "character"
	\LARGE\textbf{\opencps@reporttitle}\\[0.2cm]
	\hline
	Access\footnote{\scriptsize Access classification as per definitions in PCA; PU = Public, CO = Confidential. Access classification per deliverable stated in FPP.}:
		& \textbf{\opencps@reportaccess}\\
	\hline
	Type\footnote{\scriptsize Deliverable type according to FPP, note that all non-report deliverables must be accompanied by a deliverable report.}:
	  & \textbf{\opencps@deliverabletype}\\
	\hline
	  Version:   & \textbf{\opencps@version}\\
	\hline
	  Due Dates\footnote{\scriptsize Due month(s) according to FPP.}:
	  & \textbf{\opencps@duedate}\\
	\hline
	\multicolumn{2}{|c|}{\parbox{.5\textwidth}{\vspace{1cm}\includegraphics[width=.5\textwidth]{include/opencps-logo}}}\\[3.4cm]
	\multicolumn{2}{|c|}{\small\textit{Open Cyber-Physical System Model-Driven Certified Development}}\\[1cm]
	\hline
	\multicolumn{2}{|l|}{\textbf{Executive summary}\footnote{\scriptsize It is mandatory to provide an executive summary for each deliverable.}:}\\
	\hline
	\multicolumn{2}{|p{0.97\textwidth}|}{\opencps@executivesummary}\\
	\hline
	\end{tabularx}
 \end{center}

 \renewcommand{\arraystretch}{1.0} % set it back to default value
 \clearpage
}
