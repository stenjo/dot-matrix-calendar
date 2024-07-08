# /http_request.py size=9126
_f4 = const(
  b'# The MIT Licens'\
  b'e (MIT)\n# Copyri'\
  b'ght 2019 Jean-Ch'\
  b'ristophe Bos & H'\
  b'C2 (www.hc2.fr)\n'\
  b'\nfrom .libs.url_'\
  b'utils import Url'\
  b'Utils\nfrom .http'\
  b'_response import'\
  b' HttpResponse\nim'\
  b'port json\nimport'\
  b' sys\n\n# ========'\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'====\n# ===( Http'\
  b'Request )======='\
  b'================'\
  b'================'\
  b'================'\
  b'===\n# =========='\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'==\n\n\nclass HttpR'\
  b'equest:\n\n    MAX'\
  b'_RECV_HEADER_LIN'\
  b'ES = 100\n\n    # '\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'--------\n\n    de'\
  b'f __init__(self,'\
  b' config, xasCli,'\
  b' process_request'\
  b'):\n        self.'\
  b'_timeout_sec = c'\
  b'onfig.timeout_se'\
  b'c\n        self._'\
  b'xasCli = xasCli\n'\
  b'        self._pr'\
  b'ocess_request = '\
  b'process_request\n'\
  b'\n        self._h'\
  b'ttpVer = ""\n    '\
  b'    self._method'\
  b' = ""\n        se'\
  b'lf._path = ""\n  '\
  b'      self._head'\
  b'ers = {}\n       '\
  b' self._content ='\
  b' None\n        se'\
  b'lf._response = H'\
  b'ttpResponse(conf'\
  b'ig, self)\n\n     '\
  b'   self._recvLin'\
  b'e(self._onFirstL'\
  b'ineRecv)\n\n    # '\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'--------\n\n    de'\
  b'f _recvLine(self'\
  b', onRecv):\n     '\
  b'   self._xasCli.'\
  b'AsyncRecvLine(on'\
  b'LineRecv=onRecv,'\
  b' timeoutSec=self'\
  b'._timeout_sec)\n\n'\
  b'    # ----------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'--------------\n\n'\
  b'    def _onFirst'\
  b'LineRecv(self, x'\
  b'asCli, line, arg'\
  b'):\n        try:\n'\
  b'            elem'\
  b'ents = line.stri'\
  b'p().split()\n    '\
  b'        if len(e'\
  b'lements) == 3:\n '\
  b'               s'\
  b'elf._httpVer = e'\
  b'lements[2].upper'\
  b'()\n             '\
  b'   self._method '\
  b'= elements[0].up'\
  b'per()\n          '\
  b'      elements ='\
  b' elements[1].spl'\
  b'it("?", 1)\n     '\
  b'           self.'\
  b'_path = UrlUtils'\
  b'.UnquotePlus(ele'\
  b'ments[0])\n      '\
  b'          self._'\
  b'queryString = el'\
  b'ements[1] if len'\
  b'(elements) > 1 e'\
  b'lse ""\n         '\
  b'       self._que'\
  b'ryParams = {}\n  '\
  b'              if'\
  b' self._queryStri'\
  b'ng:\n            '\
  b'        elements'\
  b' = self._querySt'\
  b'ring.split("&")\n'\
  b'                '\
  b'    for s in ele'\
  b'ments:\n         '\
  b'               p'\
  b' = s.split("=", '\
  b'1)\n             '\
  b'           if le'\
  b'n(p) > 0:\n      '\
  b'                '\
  b'      v = UrlUti'\
  b'ls.Unquote(p[1])'\
  b' if len(p) > 1 e'\
  b'lse ""\n         '\
  b'                '\
  b'   self._queryPa'\
  b'rams[UrlUtils.Un'\
  b'quote(p[0])] = v'\
  b'\n               '\
  b' self._recvLine('\
  b'self._onHeaderLi'\
  b'neRecv)\n        '\
  b'    else:\n      '\
  b'          self._'\
  b'response.ReturnB'\
  b'adRequest()\n    '\
  b'    except Excep'\
  b'tion as e:\n     '\
  b'       sys.print'\
  b'_exception(e)\n  '\
  b'          self._'\
  b'response.ReturnB'\
  b'adRequest()\n\n   '\
  b' # -------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'-----------\n\n   '\
  b' def _onHeaderLi'\
  b'neRecv(self, xas'\
  b'Cli, line, arg):'\
  b'\n        try:\n  '\
  b'          elemen'\
  b'ts = line.strip('\
  b').split(":", 1)\n'\
  b'            if l'\
  b'en(elements) == '\
  b'2:\n             '\
  b'   if len(self._'\
  b'headers) < HttpR'\
  b'equest.MAX_RECV_'\
  b'HEADER_LINES:\n  '\
  b'                '\
  b'  self._headers['\
  b'elements[0].stri'\
  b'p().lower()] = e'\
  b'lements[1].strip'\
  b'()\n             '\
  b'       self._rec'\
  b'vLine(self._onHe'\
  b'aderLineRecv)\n  '\
  b'              el'\
  b'se:\n            '\
  b'        self._re'\
  b'sponse.ReturnEnt'\
  b'ityTooLarge()\n  '\
  b'          elif l'\
  b'en(elements) == '\
  b'1 and len(elemen'\
  b'ts[0]) == 0:\n   '\
  b'             sel'\
  b'f._process_reque'\
  b'st(self)\n       '\
  b'     else:\n     '\
  b'           self.'\
  b'_response.Return'\
  b'BadRequest()\n   '\
  b'     except Exce'\
  b'ption as e:\n    '\
  b'        sys.prin'\
  b't_exception(e)\n '\
  b'           self.'\
  b'_response.Return'\
  b'BadRequest()\n\n  '\
  b'  # ------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'------------\n\n  '\
  b'  def async_data'\
  b'_recv(self, size'\
  b', on_content_rec'\
  b'v):\n        def '\
  b'_on_content_recv'\
  b'(xasCli, content'\
  b', arg):\n        '\
  b'    self._conten'\
  b't = content\n    '\
  b'        on_conte'\
  b'nt_recv()\n      '\
  b'      self._cont'\
  b'ent = None\n\n    '\
  b'    self._xasCli'\
  b'.AsyncRecvData(\n'\
  b'            size'\
  b'=size, onDataRec'\
  b'v=_on_content_re'\
  b'cv, timeoutSec=s'\
  b'elf._timeout_sec'\
  b'\n        )\n\n    '\
  b'# --------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------\n\n    '\
  b'def GetPostedURL'\
  b'EncodedForm(self'\
  b'):\n        res ='\
  b' {}\n        if s'\
  b'elf.ContentType.'\
  b'lower() == "appl'\
  b'ication/x-www-fo'\
  b'rm-urlencoded":\n'\
  b'            try:'\
  b'\n               '\
  b' elements = byte'\
  b's(self._content)'\
  b'.decode("UTF-8")'\
  b'.split("&")\n    '\
  b'            for '\
  b's in elements:\n '\
  b'                '\
  b'   p = s.split("'\
  b'=", 1)\n         '\
  b'           if le'\
  b'n(p) > 0:\n      '\
  b'                '\
  b'  v = UrlUtils.U'\
  b'nquotePlus(p[1])'\
  b' if len(p) > 1 e'\
  b'lse ""\n         '\
  b'               r'\
  b'es[UrlUtils.Unqu'\
  b'otePlus(p[0])] ='\
  b' v\n            e'\
  b'xcept Exception '\
  b'as e:\n          '\
  b'      sys.print_'\
  b'exception(e)\n   '\
  b'     return res\n'\
  b'\n    # ---------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'---------------\n'\
  b'\n    def GetPost'\
  b'edJSONObject(sel'\
  b'f):\n        if s'\
  b'elf.ContentType.'\
  b'lower() == "appl'\
  b'ication/json":\n '\
  b'           try:\n'\
  b'                '\
  b's = bytes(self._'\
  b'content).decode('\
  b'"UTF-8")\n       '\
  b'         return '\
  b'json.loads(s)\n  '\
  b'          except'\
  b' Exception as e:'\
  b'\n               '\
  b' sys.print_excep'\
  b'tion(e)\n        '\
  b'return None\n\n   '\
  b' # -------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'-----------\n\n   '\
  b' def GetHeader(s'\
  b'elf, name):\n    '\
  b'    if not isins'\
  b'tance(name, str)'\
  b' or len(name) =='\
  b' 0:\n            '\
  b'raise ValueError'\
  b'(\'"name" must be'\
  b' a not empty str'\
  b"ing.')\n        r"\
  b'eturn self._head'\
  b'ers.get(name.low'\
  b'er(), "")\n\n    #'\
  b' ---------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'---------\n\n    @'\
  b'property\n    def'\
  b' UserAddress(sel'\
  b'f):\n        retu'\
  b'rn self._xasCli.'\
  b'CliAddr\n\n    # -'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'-------\n\n    @pr'\
  b'operty\n    def H'\
  b'ttpVer(self):\n  '\
  b'      return sel'\
  b'f._httpVer\n\n    '\
  b'# --------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------\n\n    '\
  b'@property\n    de'\
  b'f Method(self):\n'\
  b'        return s'\
  b'elf._method\n\n   '\
  b' # -------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'-----------\n\n   '\
  b' @property\n    d'\
  b'ef Path(self):\n '\
  b'       return se'\
  b'lf._path\n\n    # '\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'--------\n\n    @p'\
  b'roperty\n    def '\
  b'QueryString(self'\
  b'):\n        retur'\
  b'n self._queryStr'\
  b'ing\n\n    # -----'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'---\n\n    @proper'\
  b'ty\n    def Query'\
  b'Params(self):\n  '\
  b'      return sel'\
  b'f._queryParams\n\n'\
  b'    # ----------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'--------------\n\n'\
  b'    @property\n  '\
  b'  def Host(self)'\
  b':\n        return'\
  b' self._headers.g'\
  b'et("host", "")\n\n'\
  b'    # ----------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'--------------\n\n'\
  b'    @property\n  '\
  b'  def Accept(sel'\
  b'f):\n        s = '\
  b'self._headers.ge'\
  b't("accept", None'\
  b')\n        if s:\n'\
  b'            retu'\
  b'rn [x.strip() fo'\
  b'r x in s.split("'\
  b',")]\n        ret'\
  b'urn []\n\n    # --'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'------\n\n    @pro'\
  b'perty\n    def Ac'\
  b'ceptEncodings(se'\
  b'lf):\n        s ='\
  b' self._headers.g'\
  b'et("accept-encod'\
  b'ing", None)\n    '\
  b'    if s:\n      '\
  b'      return [x.'\
  b'strip() for x in'\
  b' s.split(",")]\n '\
  b'       return []'\
  b'\n\n    # --------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'\n\n    @property\n'\
  b'    def AcceptLa'\
  b'nguages(self):\n '\
  b'       s = self.'\
  b'_headers.get("ac'\
  b'cept-language", '\
  b'None)\n        if'\
  b' s:\n            '\
  b'return [x.strip('\
  b') for x in s.spl'\
  b'it(",")]\n       '\
  b' return []\n\n    '\
  b'# --------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------\n\n    '\
  b'@property\n    de'\
  b'f Cookies(self):'\
  b'\n        s = sel'\
  b'f._headers.get("'\
  b'cookie", None)\n '\
  b'       if s:\n   '\
  b'         return '\
  b'[x.strip() for x'\
  b' in s.split(";")'\
  b']\n        return'\
  b' []\n\n    # -----'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'---\n\n    @proper'\
  b'ty\n    def Cache'\
  b'Control(self):\n '\
  b'       return se'\
  b'lf._headers.get('\
  b'"cache-control",'\
  b' "")\n\n    # ----'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----\n\n    @prope'\
  b'rty\n    def Refe'\
  b'rer(self):\n     '\
  b'   return self._'\
  b'headers.get("ref'\
  b'erer", "")\n\n    '\
  b'# --------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------\n\n    '\
  b'@property\n    de'\
  b'f ContentType(se'\
  b'lf):\n        ret'\
  b'urn self._header'\
  b's.get("content-t'\
  b'ype", "").split('\
  b'";", 1)[0].strip'\
  b'()\n\n    # ------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'--\n\n    @propert'\
  b'y\n    def Conten'\
  b'tLength(self):\n '\
  b'       try:\n    '\
  b'        return i'\
  b'nt(self._headers'\
  b'.get("content-le'\
  b'ngth", 0))\n     '\
  b'   except:\n     '\
  b'       return 0\n'\
  b'\n    # ---------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'---------------\n'\
  b'\n    @property\n '\
  b'   def UserAgent'\
  b'(self):\n        '\
  b'return self._hea'\
  b'ders.get("user-a'\
  b'gent", "")\n\n    '\
  b'# --------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------\n\n    '\
  b'@property\n    de'\
  b'f Origin(self):\n'\
  b'        return s'\
  b'elf._headers.get'\
  b'("origin", "")\n\n'\
  b'    # ----------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'--------------\n\n'\
  b'    @property\n  '\
  b'  def IsUpgrade('\
  b'self):\n        r'\
  b'eturn "upgrade" '\
  b'in self._headers'\
  b'.get("connection'\
  b'", "").lower()\n\n'\
  b'    # ----------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'--------------\n\n'\
  b'    @property\n  '\
  b'  def Upgrade(se'\
  b'lf):\n        ret'\
  b'urn self._header'\
  b's.get("upgrade",'\
  b' "")\n\n    # ----'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----\n\n    @prope'\
  b'rty\n    def Cont'\
  b'ent(self):\n     '\
  b'   return self._'\
  b'content\n\n    # -'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'-------\n\n    @pr'\
  b'operty\n    def R'\
  b'esponse(self):\n '\
  b'       return se'\
  b'lf._response\n\n  '\
  b'  # ------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'------------\n\n  '\
  b'  @property\n    '\
  b'def XAsyncTCPCli'\
  b'ent(self):\n     '\
  b'   return self._'\
  b'xasCli\n\n\n# ====='\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'=======\n# ======'\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'======\n# ======='\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'=====\n'\
  b'')
# /http_response.py size=17605
_f0 = const(
  b'# The MIT Licens'\
  b'e (MIT)\n# Copyri'\
  b'ght 2019 Jean-Ch'\
  b'ristophe Bos & H'\
  b'C2 (www.hc2.fr)\n'\
  b'\nfrom os import '\
  b'stat\nimport json'\
  b'\nimport sys\n\nimp'\
  b'ort logging\n\n\n_l'\
  b'ogger = logging.'\
  b'getLogger("respo'\
  b'nse")\n\n\n# Read a'\
  b' file, given a p'\
  b'ath relative to '\
  b'the directory co'\
  b'ntaining this `.'\
  b'py` file.\ndef _r'\
  b'ead_relative(fil'\
  b'ename):\n    from'\
  b' shim import joi'\
  b'n, dirname, read'\
  b'_text\n\n    retur'\
  b'n read_text(join'\
  b'(dirname(__file_'\
  b'_), filename))\n\n'\
  b'\n# ============='\
  b'================'\
  b'================'\
  b'================'\
  b'===============\n'\
  b'# ===( HttpRespo'\
  b'nse )==========='\
  b'================'\
  b'================'\
  b'==============\n#'\
  b' ==============='\
  b'================'\
  b'================'\
  b'================'\
  b'=============\n\n\n'\
  b'class HttpRespon'\
  b'se:\n\n    _RESPON'\
  b'SE_CODES = {\n   '\
  b'     100: "Conti'\
  b'nue",\n        10'\
  b'1: "Switching Pr'\
  b'otocols",\n      '\
  b'  200: "OK",\n   '\
  b'     201: "Creat'\
  b'ed",\n        202'\
  b': "Accepted",\n  '\
  b'      203: "Non-'\
  b'Authoritative In'\
  b'formation",\n    '\
  b'    204: "No Con'\
  b'tent",\n        2'\
  b'05: "Reset Conte'\
  b'nt",\n        206'\
  b': "Partial Conte'\
  b'nt",\n        300'\
  b': "Multiple Choi'\
  b'ces",\n        30'\
  b'1: "Moved Perman'\
  b'ently",\n        '\
  b'302: "Found",\n  '\
  b'      303: "See '\
  b'Other",\n        '\
  b'304: "Not Modifi'\
  b'ed",\n        305'\
  b': "Use Proxy",\n '\
  b'       307: "Tem'\
  b'porary Redirect"'\
  b',\n        400: "'\
  b'Bad Request",\n  '\
  b'      401: "Unau'\
  b'thorized",\n     '\
  b'   402: "Payment'\
  b' Required",\n    '\
  b'    403: "Forbid'\
  b'den",\n        40'\
  b'4: "Not Found",\n'\
  b'        405: "Me'\
  b'thod Not Allowed'\
  b'",\n        406: '\
  b'"Not Acceptable"'\
  b',\n        407: "'\
  b'Proxy Authentica'\
  b'tion Required",\n'\
  b'        408: "Re'\
  b'quest Timeout",\n'\
  b'        409: "Co'\
  b'nflict",\n       '\
  b' 410: "Gone",\n  '\
  b'      411: "Leng'\
  b'th Required",\n  '\
  b'      412: "Prec'\
  b'ondition Failed"'\
  b',\n        413: "'\
  b'Request Entity T'\
  b'oo Large",\n     '\
  b'   414: "Request'\
  b'-URI Too Long",\n'\
  b'        415: "Un'\
  b'supported Media '\
  b'Type",\n        4'\
  b'16: "Requested R'\
  b'ange Not Satisfi'\
  b'able",\n        4'\
  b'17: "Expectation'\
  b' Failed",\n      '\
  b'  500: "Internal'\
  b' Server Error",\n'\
  b'        501: "No'\
  b't Implemented",\n'\
  b'        502: "Ba'\
  b'd Gateway",\n    '\
  b'    503: "Servic'\
  b'e Unavailable",\n'\
  b'        504: "Ga'\
  b'teway Timeout",\n'\
  b'        505: "HT'\
  b'TP Version Not S'\
  b'upported",\n    }'\
  b'\n\n    _CODE_CONT'\
  b'ENT_TMPL = _read'\
  b'_relative("statu'\
  b's-code.html")\n\n '\
  b'   # -----------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'-------------\n\n '\
  b'   def __init__('\
  b'self, config, re'\
  b'quest):\n        '\
  b'self._not_found_'\
  b'url = config.not'\
  b'_found_url\n     '\
  b'   self._allow_a'\
  b'll_origins = con'\
  b'fig.allow_all_or'\
  b'igins\n        se'\
  b'lf._server_name '\
  b'= config.server_'\
  b'name\n\n        se'\
  b'lf._request = re'\
  b'quest\n        se'\
  b'lf._xasCli = req'\
  b'uest.XAsyncTCPCl'\
  b'ient\n\n        se'\
  b'lf._headers = {}'\
  b'\n        self._a'\
  b'llowCaching = Fa'\
  b'lse\n        self'\
  b'._acAllowOrigin '\
  b'= None\n        s'\
  b'elf._contentType'\
  b' = None\n        '\
  b'self._contentCha'\
  b'rset = None\n    '\
  b'    self._conten'\
  b'tLength = 0\n    '\
  b'    self._stream'\
  b' = None\n        '\
  b'self._sendingBuf'\
  b' = None\n        '\
  b'self._hdrSent = '\
  b'False\n\n        s'\
  b'elf._switch_resu'\
  b'lt = None\n\n    #'\
  b' ---------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'---------\n\n    d'\
  b'ef SetHeader(sel'\
  b'f, name, value):'\
  b'\n        if not '\
  b'isinstance(name,'\
  b' str) or len(nam'\
  b'e) == 0:\n       '\
  b'     raise Value'\
  b'Error(\'"name" mu'\
  b'st be a not empt'\
  b"y string.')\n    "\
  b'    if value is '\
  b'None:\n          '\
  b'  raise ValueErr'\
  b'or(\'"value" cann'\
  b"ot be None.')\n  "\
  b'      self._head'\
  b'ers[name] = str('\
  b'value)\n\n    # --'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'------\n\n    def '\
  b'_onDataSent(self'\
  b', xasCli, arg):\n'\
  b'        if self.'\
  b'_stream:\n       '\
  b'     try:\n      '\
  b'          n = se'\
  b'lf._stream.readi'\
  b'nto(self._sendin'\
  b'gBuf)\n          '\
  b'      if n < len'\
  b'(self._sendingBu'\
  b'f):\n            '\
  b'        self._st'\
  b'ream.close()\n   '\
  b'                '\
  b' self._stream = '\
  b'None\n           '\
  b'         self._s'\
  b'endingBuf = self'\
  b'._sendingBuf[:n]'\
  b'\n            exc'\
  b'ept Exception as'\
  b' e:\n            '\
  b'    sys.print_ex'\
  b'ception(e)\n     '\
  b'           self.'\
  b'_xasCli.Close()\n'\
  b'                '\
  b'_logger.error(\n '\
  b'                '\
  b"   'stream canno"\
  b't be read for re'\
  b'quest "%s".\', se'\
  b'lf._request._pat'\
  b'h\n              '\
  b'  )\n            '\
  b'    return\n     '\
  b'   if self._send'\
  b'ingBuf:\n        '\
  b'    if self._con'\
  b'tentLength:\n    '\
  b'            self'\
  b'._xasCli.AsyncSe'\
  b'ndSendingBuffer('\
  b'\n               '\
  b'     size=len(se'\
  b'lf._sendingBuf),'\
  b' onDataSent=self'\
  b'._onDataSent\n   '\
  b'             )\n '\
  b'               i'\
  b'f not self._stre'\
  b'am:\n            '\
  b'        self._se'\
  b'ndingBuf = None\n'\
  b'            else'\
  b':\n\n             '\
  b'   def onChunkHd'\
  b'rSent(xasCli, ar'\
  b'g):\n            '\
  b'        def onCh'\
  b'unkDataSent(xasC'\
  b'li, arg):\n      '\
  b'                '\
  b'  def onLastChun'\
  b'kSent(xasCli, ar'\
  b'g):\n            '\
  b'                '\
  b'self._xasCli.Asy'\
  b'ncSendData(\n    '\
  b'                '\
  b'            b"0\\'\
  b'r\\n\\r\\n", onData'\
  b'Sent=self._onDat'\
  b'aSent\n          '\
  b'                '\
  b'  )\n\n           '\
  b'             if '\
  b'self._stream:\n  '\
  b'                '\
  b'          onData'\
  b'Sent = self._onD'\
  b'ataSent\n        '\
  b'                '\
  b'else:\n          '\
  b'                '\
  b'  self._sendingB'\
  b'uf = None\n      '\
  b'                '\
  b'      onDataSent'\
  b' = onLastChunkSe'\
  b'nt\n             '\
  b'           self.'\
  b'_xasCli.AsyncSen'\
  b'dData(b"\\r\\n", o'\
  b'nDataSent=onData'\
  b'Sent)\n\n         '\
  b'           self.'\
  b'_xasCli.AsyncSen'\
  b'dSendingBuffer(\n'\
  b'                '\
  b'        size=len'\
  b'(self._sendingBu'\
  b'f), onDataSent=o'\
  b'nChunkDataSent\n '\
  b'                '\
  b'   )\n\n          '\
  b'      data = ("%'\
  b'x\\r\\n" % len(sel'\
  b'f._sendingBuf)).'\
  b'encode()\n       '\
  b'         self._x'\
  b'asCli.AsyncSendD'\
  b'ata(data, onData'\
  b'Sent=onChunkHdrS'\
  b'ent)\n        els'\
  b'e:\n            s'\
  b'elf._xasCli.OnCl'\
  b'osed = None\n    '\
  b'        self._xa'\
  b'sCli.Close()\n\n  '\
  b'  # ------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'------------\n\n  '\
  b'  def _onClosed('\
  b'self, xasCli, cl'\
  b'osedReason):\n   '\
  b'     if self._st'\
  b'ream:\n          '\
  b'  try:\n         '\
  b'       self._str'\
  b'eam.close()\n    '\
  b'        except E'\
  b'xception as e:\n '\
  b'               s'\
  b'ys.print_excepti'\
  b'on(e)\n          '\
  b'  self._stream ='\
  b' None\n        se'\
  b'lf._sendingBuf ='\
  b' None\n\n    # ---'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'-----\n\n    def _'\
  b'reason(self, cod'\
  b'e):\n        retu'\
  b'rn self._RESPONS'\
  b'E_CODES.get(code'\
  b', "Unknown reaso'\
  b'n")\n\n    # -----'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'---\n\n    def _ma'\
  b'keBaseResponseHd'\
  b'r(self, code):\n '\
  b'       reason = '\
  b'self._reason(cod'\
  b'e)\n        host '\
  b'= self._request.'\
  b'Host\n        hos'\
  b't = " to {}".for'\
  b'mat(host) if hos'\
  b't else ""\n      '\
  b'  _logger.info(\n'\
  b'            "fro'\
  b'm %s:%s%s %s %s '\
  b'>> [%s] %s",\n   '\
  b'         self._x'\
  b'asCli.CliAddr[0]'\
  b',\n            se'\
  b'lf._xasCli.CliAd'\
  b'dr[1],\n         '\
  b'   host,\n       '\
  b'     self._reque'\
  b'st._method,\n    '\
  b'        self._re'\
  b'quest._path,\n   '\
  b'         code,\n '\
  b'           reaso'\
  b'n,\n        )\n   '\
  b'     if self._al'\
  b'low_all_origins:'\
  b'\n            sel'\
  b'f._acAllowOrigin'\
  b' = self._request'\
  b'.Origin\n        '\
  b'if self._acAllow'\
  b'Origin:\n        '\
  b'    self.SetHead'\
  b'er("Access-Contr'\
  b'ol-Allow-Origin"'\
  b', self._acAllowO'\
  b'rigin)\n        s'\
  b'elf.SetHeader("S'\
  b'erver", self._se'\
  b'rver_name)\n     '\
  b'   hdr = ""\n    '\
  b'    for n in sel'\
  b'f._headers:\n    '\
  b'        hdr += "'\
  b'%s: %s\\r\\n" % (n'\
  b', self._headers['\
  b'n])\n        resp'\
  b' = "HTTP/1.1 %s '\
  b'%s\\r\\n%s\\r\\n" % '\
  b'(code, reason, h'\
  b'dr)\n        retu'\
  b'rn resp.encode("'\
  b'ISO-8859-1")\n\n  '\
  b'  # ------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'------------\n\n  '\
  b'  def _makeRespo'\
  b'nseHdr(self, cod'\
  b'e):\n        self'\
  b'.SetHeader("Conn'\
  b'ection", "Close"'\
  b')\n        if sel'\
  b'f._allowCaching:'\
  b'\n            sel'\
  b'f.SetHeader("Cac'\
  b'he-Control", "pu'\
  b'blic, max-age=31'\
  b'536000")\n       '\
  b' else:\n         '\
  b'   self.SetHeade'\
  b'r("Cache-Control'\
  b'", "no-cache, no'\
  b'-store, must-rev'\
  b'alidate")\n      '\
  b'  if self._conte'\
  b'ntType:\n        '\
  b'    ct = self._c'\
  b'ontentType\n     '\
  b'       if self._'\
  b'contentCharset:\n'\
  b'                '\
  b'ct += "; charset'\
  b'=%s" % self._con'\
  b'tentCharset\n    '\
  b'        self.Set'\
  b'Header("Content-'\
  b'Type", ct)\n     '\
  b'   if self._cont'\
  b'entLength:\n     '\
  b'       self.SetH'\
  b'eader("Content-L'\
  b'ength", self._co'\
  b'ntentLength)\n   '\
  b'     return self'\
  b'._makeBaseRespon'\
  b'seHdr(code)\n\n   '\
  b' # -------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'-----------\n\n   '\
  b' def _on_switche'\
  b'd(self, xas_cli,'\
  b' _):\n        if '\
  b'not self._switch'\
  b'_result:\n       '\
  b'     return\n\n   '\
  b'     self._switc'\
  b'h_result(xas_cli'\
  b'.detach_socket()'\
  b')\n        self._'\
  b'switch_result = '\
  b'None\n\n    def Sw'\
  b'itchingProtocols'\
  b'(self, upgrade, '\
  b'switch_result=No'\
  b'ne):\n        sel'\
  b'f._switch_result'\
  b' = switch_result'\
  b'\n        if not '\
  b'isinstance(upgra'\
  b'de, str) or len('\
  b'upgrade) == 0:\n '\
  b'           raise'\
  b' ValueError(\'"up'\
  b'grade" must be a'\
  b' not empty strin'\
  b"g.')\n        if "\
  b'self._hdrSent:\n '\
  b'           _logg'\
  b'er.warning(\n    '\
  b"            'res"\
  b'ponse headers al'\
  b'ready sent for r'\
  b'equest "%s".\', s'\
  b'elf._request._pa'\
  b'th\n            )'\
  b'\n            ret'\
  b'urn\n        self'\
  b'.SetHeader("Conn'\
  b'ection", "Upgrad'\
  b'e")\n        self'\
  b'.SetHeader("Upgr'\
  b'ade", upgrade)\n '\
  b'       data = se'\
  b'lf._makeBaseResp'\
  b'onseHdr(101)\n   '\
  b'     self._xasCl'\
  b'i.AsyncSendData('\
  b'data, self._on_s'\
  b'witched)\n       '\
  b' self._hdrSent ='\
  b' True\n\n    # ---'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'-----\n\n    def R'\
  b'eturnStream(self'\
  b', code, stream):'\
  b'\n        if not '\
  b'isinstance(code,'\
  b' int) or code <='\
  b' 0:\n            '\
  b'raise ValueError'\
  b'(\'"code" must be'\
  b' a positive inte'\
  b"ger.')\n        i"\
  b'f not hasattr(st'\
  b'ream, "readinto"'\
  b') or not hasattr'\
  b'(stream, "close"'\
  b'):\n            r'\
  b'aise ValueError('\
  b'\'"stream" must b'\
  b'e a readable buf'\
  b'fer protocol obj'\
  b"ect.')\n        i"\
  b'f self._hdrSent:'\
  b'\n            _lo'\
  b'gger.warning(\n  '\
  b"              'r"\
  b'esponse headers '\
  b'already sent for'\
  b' request "%s".\','\
  b' self._request._'\
  b'path\n           '\
  b' )\n            t'\
  b'ry:\n            '\
  b'    stream.close'\
  b'()\n            e'\
  b'xcept Exception '\
  b'as e:\n          '\
  b'      sys.print_'\
  b'exception(e)\n   '\
  b'         return\n'\
  b'        if self.'\
  b'_request._method'\
  b' != "HEAD":\n    '\
  b'        self._st'\
  b'ream = stream\n  '\
  b'          self._'\
  b'sendingBuf = mem'\
  b'oryview(self._xa'\
  b'sCli.SendingBuff'\
  b'er)\n            '\
  b'self._xasCli.OnC'\
  b'losed = self._on'\
  b'Closed\n        e'\
  b'lse:\n           '\
  b' try:\n          '\
  b'      stream.clo'\
  b'se()\n           '\
  b' except Exceptio'\
  b'n as e:\n        '\
  b'        sys.prin'\
  b't_exception(e)\n '\
  b'       if not se'\
  b'lf._contentType:'\
  b'\n            sel'\
  b'f._contentType ='\
  b' "application/oc'\
  b'tet-stream"\n    '\
  b'    if not self.'\
  b'_contentLength:\n'\
  b'            self'\
  b'.SetHeader("Tran'\
  b'sfer-Encoding", '\
  b'"chunked")\n     '\
  b'   data = self._'\
  b'makeResponseHdr('\
  b'code)\n        se'\
  b'lf._xasCli.Async'\
  b'SendData(data, o'\
  b'nDataSent=self._'\
  b'onDataSent)\n    '\
  b'    self._hdrSen'\
  b't = True\n\n    # '\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'--------\n\n    # '\
  b'An accept header'\
  b' can contain pat'\
  b'terns, e.g. "tex'\
  b't/*" but this fu'\
  b'nction only hand'\
  b'les the pattern '\
  b'"*/*".\n    def _'\
  b'status_code_cont'\
  b'ent(self, code):'\
  b'\n        for typ'\
  b'e in self._reque'\
  b'st.Accept:\n     '\
  b'       type = ty'\
  b'pe.rsplit(";", 1'\
  b')[0]  # Strip ";'\
  b'q=weight".\n     '\
  b'       if type i'\
  b'n ["text/html", '\
  b'"*/*"]:\n        '\
  b'        content '\
  b'= self._CODE_CON'\
  b'TENT_TMPL.format'\
  b'(\n              '\
  b'      code=code,'\
  b' reason=self._re'\
  b'ason(code)\n     '\
  b'           )\n   '\
  b'             ret'\
  b'urn "text/html",'\
  b' content\n       '\
  b'     if type == '\
  b'"application/jso'\
  b'n":\n            '\
  b'    content = {"'\
  b'code": code, "na'\
  b'me": self._reaso'\
  b'n(code)}\n       '\
  b'         return '\
  b'"application/jso'\
  b'n", json.dumps(c'\
  b'ontent)\n        '\
  b'return None, Non'\
  b'e\n\n    def Retur'\
  b'n(self, code, co'\
  b'ntent=None):\n   '\
  b'     if not isin'\
  b'stance(code, int'\
  b') or code <= 0:\n'\
  b'            rais'\
  b'e ValueError(\'"c'\
  b'ode" must be a p'\
  b'ositive integer.'\
  b"')\n        if se"\
  b'lf._hdrSent:\n   '\
  b'         _logger'\
  b'.warning(\n      '\
  b"          'respo"\
  b'nse headers alre'\
  b'ady sent for req'\
  b'uest "%s".\', sel'\
  b'f._request._path'\
  b'\n            )\n '\
  b'           retur'\
  b'n\n        if not'\
  b' content:\n      '\
  b'      (self._con'\
  b'tentType, conten'\
  b't) = self._statu'\
  b's_code_content(c'\
  b'ode)\n\n        if'\
  b' content:\n      '\
  b'      if isinsta'\
  b'nce(content, str'\
  b'):\n             '\
  b'   content = con'\
  b'tent.encode("UTF'\
  b'-8")\n           '\
  b'     if not self'\
  b'._contentType:\n '\
  b'                '\
  b'   self._content'\
  b'Type = "text/htm'\
  b'l"\n             '\
  b'   self._content'\
  b'Charset = "UTF-8'\
  b'"\n            el'\
  b'if not self._con'\
  b'tentType:\n      '\
  b'          self._'\
  b'contentType = "a'\
  b'pplication/octet'\
  b'-stream"\n       '\
  b'     self._conte'\
  b'ntLength = len(c'\
  b'ontent)\n\n       '\
  b' data = self._ma'\
  b'keResponseHdr(co'\
  b'de)\n\n        if '\
  b'content and self'\
  b'._request._metho'\
  b'd != "HEAD":\n   '\
  b'         data +='\
  b' bytes(content)\n'\
  b'\n        self._x'\
  b'asCli.AsyncSendD'\
  b'ata(data, onData'\
  b'Sent=self._onDat'\
  b'aSent)\n        s'\
  b'elf._hdrSent = T'\
  b'rue\n\n    # -----'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'---\n\n    def Ret'\
  b'urnJSON(self, co'\
  b'de, obj):\n      '\
  b'  if not isinsta'\
  b'nce(code, int) o'\
  b'r code <= 0:\n   '\
  b'         raise V'\
  b'alueError(\'"code'\
  b'" must be a posi'\
  b"tive integer.')\n"\
  b'        self._co'\
  b'ntentType = "app'\
  b'lication/json"\n '\
  b'       try:\n    '\
  b'        content '\
  b'= json.dumps(obj'\
  b')\n        except'\
  b':\n            ra'\
  b"ise ValueError('"\
  b'"obj" cannot be '\
  b'converted into J'\
  b"SON format.')\n  "\
  b'      self.Retur'\
  b'n(code, content)'\
  b'\n\n    # --------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'\n\n    def Return'\
  b'Ok(self, content'\
  b'=None):\n        '\
  b'self.Return(200,'\
  b' content)\n\n    #'\
  b' ---------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'---------\n\n    d'\
  b'ef ReturnOkJSON('\
  b'self, obj):\n    '\
  b'    self.ReturnJ'\
  b'SON(200, obj)\n\n '\
  b'   # -----------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'-------------\n\n '\
  b'   def ReturnFil'\
  b'e(self, filename'\
  b', attachmentName'\
  b'=None):\n        '\
  b'if not isinstanc'\
  b'e(filename, str)'\
  b' or len(filename'\
  b') == 0:\n        '\
  b'    raise ValueE'\
  b'rror(\'"filename"'\
  b' must be a not e'\
  b"mpty string.')\n "\
  b'       if attach'\
  b'mentName is not '\
  b'None and not isi'\
  b'nstance(attachme'\
  b'ntName, str):\n  '\
  b'          raise '\
  b'ValueError(\'"att'\
  b'achmentName" mus'\
  b't be a string or'\
  b" None.')\n       "\
  b' try:\n          '\
  b'  size = stat(fi'\
  b'lename)[6]\n     '\
  b'   except:\n     '\
  b'       self.Retu'\
  b'rnNotFound()\n   '\
  b'         return\n'\
  b'        try:\n   '\
  b'         file = '\
  b'open(filename, "'\
  b'rb")\n        exc'\
  b'ept:\n           '\
  b' self.ReturnForb'\
  b'idden()\n        '\
  b'    return\n     '\
  b'   if attachment'\
  b'Name:\n          '\
  b"  cd = 'attachme"\
  b'nt; filename="%s'\
  b'"\' % attachmentN'\
  b'ame.replace(\'"\','\
  b' "\'")\n          '\
  b'  self.SetHeader'\
  b'("Content-Dispos'\
  b'ition", cd)\n    '\
  b'    if not self.'\
  b'_contentType:\n  '\
  b'          raise '\
  b'ValueError(\'"Con'\
  b'tentType" must b'\
  b"e set')\n        "\
  b'self._contentLen'\
  b'gth = size\n     '\
  b'   self.ReturnSt'\
  b'ream(200, file)\n'\
  b'\n    # ---------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'---------------\n'\
  b'\n    def ReturnN'\
  b'otModified(self)'\
  b':\n        self.R'\
  b'eturn(304)\n\n    '\
  b'# --------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------\n\n    '\
  b'def ReturnRedire'\
  b'ct(self, locatio'\
  b'n):\n        if n'\
  b'ot isinstance(lo'\
  b'cation, str) or '\
  b'len(location) =='\
  b' 0:\n            '\
  b'raise ValueError'\
  b'(\'"location" mus'\
  b't be a not empty'\
  b" string.')\n     "\
  b'   self.SetHeade'\
  b'r("Location", lo'\
  b'cation)\n        '\
  b'self.Return(307)'\
  b'\n\n    # --------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'\n\n    def Return'\
  b'BadRequest(self)'\
  b':\n        self.R'\
  b'eturn(400)\n\n    '\
  b'# --------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------\n\n    '\
  b'def ReturnForbid'\
  b'den(self):\n     '\
  b'   self.Return(4'\
  b'03)\n\n    # -----'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'---\n\n    def Ret'\
  b'urnNotFound(self'\
  b'):\n        if se'\
  b'lf._not_found_ur'\
  b'l:\n            s'\
  b'elf.ReturnRedire'\
  b'ct(self._not_fou'\
  b'nd_url)\n        '\
  b'else:\n          '\
  b'  self.Return(40'\
  b'4)\n\n    # ------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'--\n\n    def Retu'\
  b'rnMethodNotAllow'\
  b'ed(self):\n      '\
  b'  self.Return(40'\
  b'5)\n\n    # ------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'--\n\n    def Retu'\
  b'rnEntityTooLarge'\
  b'(self):\n        '\
  b'self.Return(413)'\
  b'\n\n    # --------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'\n\n    def Return'\
  b'InternalServerEr'\
  b'ror(self):\n     '\
  b'   self.Return(5'\
  b'00)\n\n    # -----'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'---\n\n    def Ret'\
  b'urnNotImplemente'\
  b'd(self):\n       '\
  b' self.Return(501'\
  b')\n\n    # -------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'-\n\n    def Retur'\
  b'nServiceUnavaila'\
  b'ble(self):\n     '\
  b'   self.Return(5'\
  b'03)\n\n    # -----'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'---\n\n    @proper'\
  b'ty\n    def Reque'\
  b'st(self):\n      '\
  b'  return self._r'\
  b'equest\n\n    # --'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'------\n\n    @pro'\
  b'perty\n    def Us'\
  b'erAddress(self):'\
  b'\n        return '\
  b'self._xasCli.Cli'\
  b'Addr\n\n    # ----'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----\n\n    @prope'\
  b'rty\n    def Allo'\
  b'wCaching(self):\n'\
  b'        return s'\
  b'elf._allowCachin'\
  b'g\n\n    @AllowCac'\
  b'hing.setter\n    '\
  b'def AllowCaching'\
  b'(self, value):\n '\
  b'       self._che'\
  b'ck_value("AllowC'\
  b'aching", value, '\
  b'isinstance(value'\
  b', bool))\n       '\
  b' self._allowCach'\
  b'ing = value\n\n   '\
  b' # -------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'-----------\n\n   '\
  b' @property\n    d'\
  b'ef AccessControl'\
  b'AllowOrigin(self'\
  b'):\n        retur'\
  b'n self._acAllowO'\
  b'rigin\n\n    @Acce'\
  b'ssControlAllowOr'\
  b'igin.setter\n    '\
  b'def AccessContro'\
  b'lAllowOrigin(sel'\
  b'f, value):\n     '\
  b'   self._check_n'\
  b'one_or_str("Acce'\
  b'ssControlAllowOr'\
  b'igin", value)\n  '\
  b'      self._acAl'\
  b'lowOrigin = valu'\
  b'e\n\n    # -------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'-\n\n    @property'\
  b'\n    def Content'\
  b'Type(self):\n    '\
  b'    return self.'\
  b'_contentType\n\n  '\
  b'  @ContentType.s'\
  b'etter\n    def Co'\
  b'ntentType(self, '\
  b'value):\n        '\
  b'self._check_none'\
  b'_or_str("Content'\
  b'Type", value)\n  '\
  b'      self._cont'\
  b'entType = value\n'\
  b'\n    # ---------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'---------------\n'\
  b'\n    @property\n '\
  b'   def ContentCh'\
  b'arset(self):\n   '\
  b'     return self'\
  b'._contentCharset'\
  b'\n\n    @ContentCh'\
  b'arset.setter\n   '\
  b' def ContentChar'\
  b'set(self, value)'\
  b':\n        self._'\
  b'check_none_or_st'\
  b'r("ContentCharse'\
  b't", value)\n     '\
  b'   self._content'\
  b'Charset = value\n'\
  b'\n    # ---------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'---------------\n'\
  b'\n    @property\n '\
  b'   def ContentLe'\
  b'ngth(self):\n    '\
  b'    return self.'\
  b'_contentLength\n\n'\
  b'    @ContentLeng'\
  b'th.setter\n    de'\
  b'f ContentLength('\
  b'self, value):\n  '\
  b'      self._chec'\
  b'k_value("Content'\
  b'Length", value, '\
  b'isinstance(value'\
  b', int) and value'\
  b' >= 0)\n        s'\
  b'elf._contentLeng'\
  b'th = value\n\n    '\
  b'# --------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------\n\n    '\
  b'@property\n    de'\
  b'f HeadersSent(se'\
  b'lf):\n        ret'\
  b'urn self._hdrSen'\
  b't\n\n    # -------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'-\n\n    def _chec'\
  b'k_value(self, na'\
  b'me, value, condi'\
  b'tion):\n        i'\
  b'f not condition:'\
  b'\n            rai'\
  b"se ValueError('{"\
  b'} is not a valid'\
  b' value for "{}"\''\
  b'.format(value, n'\
  b'ame))\n\n    def _'\
  b'check_none_or_st'\
  b'r(self, name, va'\
  b'lue):\n        se'\
  b'lf._check_value('\
  b'name, value, val'\
  b'ue is None or is'\
  b'instance(value, '\
  b'str))\n\n\n# ======'\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'======\n# ======='\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'=====\n# ========'\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'====\n'\
  b'')
# /libs/url_utils.py size=1010
_f2 = const(
  b'# The MIT Licens'\
  b'e (MIT)\n# Copyri'\
  b'ght 2019 Jean-Ch'\
  b'ristophe Bos & H'\
  b'C2 (www.hc2.fr)\n'\
  b'\n\nclass UrlUtils'\
  b':\n\n    # -------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'-----\n\n    @stat'\
  b'icmethod\n    def'\
  b' Unquote(s):\n   '\
  b'     r = str(s).'\
  b'split("%")\n     '\
  b'   try:\n        '\
  b'    b = r[0].enc'\
  b'ode()\n          '\
  b'  for i in range'\
  b'(1, len(r)):\n   '\
  b'             try'\
  b':\n              '\
  b'      b += bytes'\
  b'([int(r[i][:2], '\
  b'16)]) + r[i][2:]'\
  b'.encode()\n      '\
  b'          except'\
  b':\n              '\
  b'      b += b"%" '\
  b'+ r[i].encode()\n'\
  b'            retu'\
  b'rn b.decode("UTF'\
  b'-8")\n        exc'\
  b'ept:\n           '\
  b' return str(s)\n\n'\
  b'    # ----------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'--\n\n    @staticm'\
  b'ethod\n    def Un'\
  b'quotePlus(s):\n  '\
  b'      return Url'\
  b'Utils.Unquote(st'\
  b'r(s).replace("+"'\
  b', " "))\n\n    # ='\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'===========\n    '\
  b'# =============='\
  b'================'\
  b'================'\
  b'================'\
  b'==============\n '\
  b'   # ==========='\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'=\n'\
  b'')
# /libs/xasync_sockets.py size=17044
_f1 = const(
  b'# The MIT Licens'\
  b'e (MIT)\n# Copyri'\
  b'ght 2019 Jean-Ch'\
  b'ristophe Bos & H'\
  b'C2 (www.hc2.fr)\n'\
  b'\n\nimport sys\nfro'\
  b'm time import ti'\
  b'cks_ms, ticks_di'\
  b'ff, ticks_add\n\ni'\
  b'mport logging\n\n\n'\
  b'_logger = loggin'\
  b'g.getLogger("xas'\
  b'ync")\n\n\n# ======'\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'======\n# ===( XC'\
  b'losedReason )==='\
  b'================'\
  b'================'\
  b'================'\
  b'=====\n# ========'\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'====\n\n\nclass XCl'\
  b'osedReason:\n    '\
  b'Error = 0x00\n   '\
  b' ClosedByHost = '\
  b'0x01\n    ClosedB'\
  b'yPeer = 0x02\n   '\
  b' Timeout = 0x03\n'\
  b'    Detached = 0'\
  b'x04\n\n\n# ========'\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'====\n# ===( XAsy'\
  b'ncSocket )======'\
  b'================'\
  b'================'\
  b'================'\
  b'===\n# =========='\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'==\n\n\nclass XAsyn'\
  b'cSocketException'\
  b'(Exception):\n   '\
  b' pass\n\n\nclass XA'\
  b'syncSocket:\n    '\
  b'def __init__(sel'\
  b'f, asyncSocketsP'\
  b'ool, socket, rec'\
  b'vBufSlot=None, s'\
  b'endBufSlot=None)'\
  b':\n        if typ'\
  b'e(self) is XAsyn'\
  b'cSocket:\n       '\
  b'     raise XAsyn'\
  b'cSocketException'\
  b'(\n              '\
  b'  "XAsyncSocket '\
  b'is an abstract c'\
  b'lass and must be'\
  b' implemented."\n '\
  b'           )\n   '\
  b'     self._async'\
  b'SocketsPool = as'\
  b'yncSocketsPool\n '\
  b'       self._soc'\
  b'ket = socket\n   '\
  b'     self._recvB'\
  b'ufSlot = recvBuf'\
  b'Slot\n        sel'\
  b'f._sendBufSlot ='\
  b' sendBufSlot\n   '\
  b'     self._expir'\
  b'e_time_millis = '\
  b'None\n        sel'\
  b'f._onClosed = No'\
  b'ne\n        try:\n'\
  b'            sock'\
  b'et.settimeout(0)'\
  b'\n            soc'\
  b'ket.setblocking('\
  b'0)\n            i'\
  b'f (recvBufSlot i'\
  b's not None and t'\
  b'ype(recvBufSlot)'\
  b' is not XBufferS'\
  b'lot) or (\n      '\
  b'          sendBu'\
  b'fSlot is not Non'\
  b'e and type(sendB'\
  b'ufSlot) is not X'\
  b'BufferSlot\n     '\
  b'       ):\n      '\
  b'          raise '\
  b'Exception()\n    '\
  b'        asyncSoc'\
  b'ketsPool.AddAsyn'\
  b'cSocket(self)\n  '\
  b'      except Exc'\
  b'eption as e:\n   '\
  b'         sys.pri'\
  b'nt_exception(e)\n'\
  b'            rais'\
  b'e XAsyncSocketEx'\
  b'ception("XAsyncS'\
  b'ocket : Argument'\
  b's are incorrects'\
  b'.")\n\n    # -----'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'---\n\n    def _se'\
  b'tExpireTimeout(s'\
  b'elf, timeoutSec)'\
  b':\n        try:\n '\
  b'           if ti'\
  b'meoutSec and tim'\
  b'eoutSec > 0:\n   '\
  b'             sel'\
  b'f._expire_time_m'\
  b'illis = ticks_ad'\
  b'd(ticks_ms(), ti'\
  b'meoutSec * 1000)'\
  b'\n        except:'\
  b'\n            rai'\
  b'se XAsyncSocketE'\
  b'xception(\n      '\
  b'          \'"time'\
  b'outSec" is incor'\
  b'rect to set expi'\
  b"re timeout.'\n   "\
  b'         )\n\n    '\
  b'# --------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------\n\n    '\
  b'def _removeExpir'\
  b'eTimeout(self):\n'\
  b'        self._ex'\
  b'pire_time_millis'\
  b' = None\n\n    # -'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'-------\n\n    # A'\
  b' subclass can ch'\
  b'oose to do somet'\
  b'hing on socket e'\
  b'xpiration.\n    d'\
  b'ef _expired(self'\
  b'):\n        pass\n'\
  b'\n    # ---------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'---------------\n'\
  b'\n    def pump_ex'\
  b'pire(self):\n    '\
  b'    if self._exp'\
  b'ire_time_millis:'\
  b'\n            dif'\
  b'f = ticks_diff(t'\
  b'icks_ms(), self.'\
  b'_expire_time_mil'\
  b'lis)\n           '\
  b' if diff > 0:\n  '\
  b'              se'\
  b'lf._expired()\n  '\
  b'              se'\
  b'lf._close(XClose'\
  b'dReason.Timeout)'\
  b'\n\n    # --------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'\n\n    def detach'\
  b'_socket(self):\n '\
  b'       socket = '\
  b'self._socket\n   '\
  b'     if not self'\
  b'._close(XClosedR'\
  b'eason.Detached, '\
  b'do_close=False):'\
  b'\n            rai'\
  b'se XAsyncSocketE'\
  b'xception("Failed'\
  b' to detach socke'\
  b't")\n        retu'\
  b'rn socket\n\n    d'\
  b'ef _close(\n     '\
  b'   self, closedR'\
  b'eason=XClosedRea'\
  b'son.Error, trigg'\
  b'erOnClosed=True,'\
  b' do_close=True\n '\
  b'   ):\n        if'\
  b' self._asyncSock'\
  b'etsPool.RemoveAs'\
  b'yncSocket(self):'\
  b'\n            try'\
  b':\n              '\
  b'  if do_close:\n '\
  b'                '\
  b'   self._socket.'\
  b'close()\n        '\
  b'    except Excep'\
  b'tion as e:\n     '\
  b'           sys.p'\
  b'rint_exception(e'\
  b')\n            se'\
  b'lf._socket = Non'\
  b'e\n            if'\
  b' self._recvBufSl'\
  b'ot is not None:\n'\
  b'                '\
  b'self._recvBufSlo'\
  b't = None\n       '\
  b'     if self._se'\
  b'ndBufSlot is not'\
  b' None:\n         '\
  b'       self._sen'\
  b'dBufSlot = None\n'\
  b'            if t'\
  b'riggerOnClosed a'\
  b'nd self._onClose'\
  b'd:\n             '\
  b'   try:\n        '\
  b'            self'\
  b'._onClosed(self,'\
  b' closedReason)\n '\
  b'               e'\
  b'xcept Exception '\
  b'as ex:\n         '\
  b'           raise'\
  b' XAsyncSocketExc'\
  b'eption(\n        '\
  b'                '\
  b"'Error when hand"\
  b'ling the "OnClos'\
  b'e" event : %s\' %'\
  b' ex\n            '\
  b'        )\n      '\
  b'      return Tru'\
  b'e\n        return'\
  b' False\n\n    # --'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'------\n\n    def '\
  b'GetSocketObj(sel'\
  b'f):\n        retu'\
  b'rn self._socket\n'\
  b'\n    # ---------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'---------------\n'\
  b'\n    def Close(s'\
  b'elf):\n        re'\
  b'turn self._close'\
  b'(XClosedReason.C'\
  b'losedByHost)\n\n  '\
  b'  # ------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'------------\n\n  '\
  b'  def OnReadyFor'\
  b'Reading(self):\n '\
  b'       pass\n\n   '\
  b' # -------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'-----------\n\n   '\
  b' def OnReadyForW'\
  b'riting(self):\n  '\
  b'      pass\n\n    '\
  b'# --------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------\n\n    '\
  b'def OnExceptiona'\
  b'lCondition(self)'\
  b':\n        self._'\
  b'close()\n\n    # -'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'-------\n\n    @pr'\
  b'operty\n    def E'\
  b'xpireTimeSec(sel'\
  b'f):\n        retu'\
  b'rn self._expire_'\
  b'time_millis / 10'\
  b'00\n\n    @propert'\
  b'y\n    def OnClos'\
  b'ed(self):\n      '\
  b'  return self._o'\
  b'nClosed\n\n    @On'\
  b'Closed.setter\n  '\
  b'  def OnClosed(s'\
  b'elf, value):\n   '\
  b'     self._onClo'\
  b'sed = value\n\n\n# '\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'============\n# ='\
  b'==( XAsyncTCPCli'\
  b'ent )==========='\
  b'================'\
  b'================'\
  b'===========\n# =='\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'==========\n\n\ncla'\
  b'ss XAsyncTCPClie'\
  b'ntException(Exce'\
  b'ption):\n    pass'\
  b'\n\n\nclass XAsyncT'\
  b'CPClient(XAsyncS'\
  b'ocket):\n    def '\
  b'__init__(self, a'\
  b'syncSocketsPool,'\
  b' cliSocket, cliA'\
  b'ddr, recvBufSlot'\
  b', sendBufSlot):\n'\
  b'        try:\n   '\
  b'         super()'\
  b'.__init__(asyncS'\
  b'ocketsPool, cliS'\
  b'ocket, recvBufSl'\
  b'ot, sendBufSlot)'\
  b'\n            sel'\
  b'f._cliAddr = cli'\
  b'Addr if cliAddr '\
  b'else ("0.0.0.0",'\
  b' 0)\n            '\
  b'self._onFailsToC'\
  b'onnect = None\n  '\
  b'          self._'\
  b'onConnected = No'\
  b'ne\n            s'\
  b'elf._onDataRecv '\
  b'= None\n         '\
  b'   self._onDataR'\
  b'ecvArg = None\n  '\
  b'          self._'\
  b'onDataSent = Non'\
  b'e\n            se'\
  b'lf._onDataSentAr'\
  b'g = None\n       '\
  b'     self._sizeT'\
  b'oRecv = None\n   '\
  b'         self._r'\
  b'dLinePos = None\n'\
  b'            self'\
  b'._rdLineEncoding'\
  b' = None\n        '\
  b'    self._rdBufV'\
  b'iew = None\n     '\
  b'       self._wrB'\
  b'ufView = None\n  '\
  b'      except Exc'\
  b'eption as e:\n   '\
  b'         sys.pri'\
  b'nt_exception(e)\n'\
  b'            rais'\
  b'e XAsyncTCPClien'\
  b'tException(\n    '\
  b'            "Err'\
  b'or to creating X'\
  b'AsyncTCPClient, '\
  b'arguments are in'\
  b'corrects."\n     '\
  b'       )\n\n    # '\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'--------\n\n    de'\
  b'f _expired(self)'\
  b':\n        # This'\
  b' actually happen'\
  b's regularly. It '\
  b'seems to be a sp'\
  b'eed trick used b'\
  b'y browsers,\n    '\
  b'    # they open '\
  b'multiple concurr'\
  b'ent connections '\
  b'in _anticipation'\
  b'_ of needing the'\
  b'm for\n        # '\
  b'additional reque'\
  b'sts, e.g. as a p'\
  b'age loads. Some '\
  b'of these connect'\
  b'ions are never\n '\
  b'       # used an'\
  b'd this eventuall'\
  b'y triggers the e'\
  b'xpiration logic '\
  b'here.\n        _l'\
  b'ogger.debug(\n   '\
  b'         "connec'\
  b'tion from %s:%s '\
  b'expired", self._'\
  b'cliAddr[0], self'\
  b'._cliAddr[1]\n   '\
  b'     )\n\n    # --'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'------\n\n    def '\
  b'Close(self):\n   '\
  b'     if self._wr'\
  b'BufView:\n       '\
  b'     try:\n      '\
  b'          self._'\
  b'socket.send(self'\
  b'._wrBufView)\n   '\
  b'         except '\
  b'Exception as e:\n'\
  b'                '\
  b'sys.print_except'\
  b'ion(e)\n        r'\
  b'eturn self._clos'\
  b'e(XClosedReason.'\
  b'ClosedByHost)\n\n '\
  b'   # -----------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'-------------\n\n '\
  b'   def OnReadyFo'\
  b'rReading(self):\n'\
  b'        while Tr'\
  b'ue:\n            '\
  b'if self._rdLineP'\
  b'os is not None:\n'\
  b'                '\
  b'# In the context'\
  b' of reading a li'\
  b'ne,\n            '\
  b'    while True:\n'\
  b'                '\
  b'    try:\n       '\
  b'                '\
  b' try:\n          '\
  b'                '\
  b'  b = self._sock'\
  b'et.recv(1)\n     '\
  b'                '\
  b'   except Blocki'\
  b'ngIOError as bio'\
  b'Err:\n           '\
  b'                '\
  b' if bioErr.errno'\
  b' != 35:\n        '\
  b'                '\
  b'        self._cl'\
  b'ose()\n          '\
  b'                '\
  b'  return\n       '\
  b'                '\
  b' except:\n       '\
  b'                '\
  b'     self._close'\
  b'()\n             '\
  b'               r'\
  b'eturn\n          '\
  b'          except'\
  b':\n              '\
  b'          self._'\
  b'close()\n        '\
  b'                '\
  b'return\n         '\
  b'           if b:'\
  b'\n               '\
  b'         if b =='\
  b' b"\\n":\n        '\
  b'                '\
  b'    lineLen = se'\
  b'lf._rdLinePos\n  '\
  b'                '\
  b'          self._'\
  b'rdLinePos = None'\
  b'\n               '\
  b'             sel'\
  b'f._asyncSocketsP'\
  b'ool.NotifyNextRe'\
  b'adyForReading(\n '\
  b'                '\
  b'               s'\
  b'elf, False\n     '\
  b'                '\
  b'       )\n       '\
  b'                '\
  b'     self._remov'\
  b'eExpireTimeout()'\
  b'\n               '\
  b'             if '\
  b'self._onDataRecv'\
  b':\n              '\
  b'                '\
  b'  line = self._r'\
  b'ecvBufSlot.Buffe'\
  b'r[:lineLen]\n    '\
  b'                '\
  b'            try:'\
  b'\n               '\
  b'                '\
  b'     line = byte'\
  b's(line).decode(s'\
  b'elf._rdLineEncod'\
  b'ing)\n           '\
  b'                '\
  b'     except:\n   '\
  b'                '\
  b'                '\
  b' line = None\n   '\
  b'                '\
  b'             try'\
  b':\n              '\
  b'                '\
  b'      self._onDa'\
  b'taRecv(self, lin'\
  b'e, self._onDataR'\
  b'ecvArg)\n        '\
  b'                '\
  b'        except E'\
  b'xception as ex:\n'\
  b'                '\
  b'                '\
  b'    sys.print_ex'\
  b'ception(ex)\n    '\
  b'                '\
  b'                '\
  b'raise XAsyncTCPC'\
  b'lientException(\n'\
  b'                '\
  b'                '\
  b"        'Error w"\
  b'hen handling the'\
  b' "OnDataRecv" ev'\
  b"ent : %s'\n      "\
  b'                '\
  b'                '\
  b'  % ex\n         '\
  b'                '\
  b'           )\n   '\
  b'                '\
  b'         return\n'\
  b'                '\
  b'        elif b !'\
  b'= b"\\r":\n       '\
  b'                '\
  b'     if self._rd'\
  b'LinePos < self._'\
  b'recvBufSlot.Size'\
  b':\n              '\
  b'                '\
  b'  self._recvBufS'\
  b'lot.Buffer[self.'\
  b'_rdLinePos] = or'\
  b'd(b)\n           '\
  b'                '\
  b'     self._rdLin'\
  b'ePos += 1\n      '\
  b'                '\
  b'      else:\n    '\
  b'                '\
  b'            self'\
  b'._close()\n      '\
  b'                '\
  b'          return'\
  b'\n               '\
  b'     else:\n     '\
  b'                '\
  b'   self._close(X'\
  b'ClosedReason.Clo'\
  b'sedByPeer)\n     '\
  b'                '\
  b'   return\n      '\
  b'      elif self.'\
  b'_sizeToRecv:\n   '\
  b'             # I'\
  b'n the context of'\
  b' reading data,\n '\
  b'               r'\
  b'ecvBuf = self._r'\
  b'dBufView[-self._'\
  b'sizeToRecv :]\n  '\
  b'              tr'\
  b'y:\n             '\
  b'       try:\n    '\
  b'                '\
  b'    n = self._so'\
  b'cket.recv_into(r'\
  b'ecvBuf)\n        '\
  b'            exce'\
  b'pt BlockingIOErr'\
  b'or as bioErr:\n  '\
  b'                '\
  b'      if bioErr.'\
  b'errno != 35:\n   '\
  b'                '\
  b'         self._c'\
  b'lose()\n         '\
  b'               r'\
  b'eturn\n          '\
  b'          except'\
  b':\n              '\
  b'          self._'\
  b'close()\n        '\
  b'                '\
  b'return\n         '\
  b'       except:\n '\
  b'                '\
  b'   try:\n        '\
  b'                '\
  b'n = self._socket'\
  b'.readinto(recvBu'\
  b'f)\n             '\
  b'       except:\n '\
  b'                '\
  b'       self._clo'\
  b'se()\n           '\
  b'             ret'\
  b'urn\n            '\
  b'    if not n:\n  '\
  b'                '\
  b'  self._close(XC'\
  b'losedReason.Clos'\
  b'edByPeer)\n      '\
  b'              re'\
  b'turn\n           '\
  b'     self._sizeT'\
  b'oRecv -= n\n     '\
  b'           if no'\
  b't self._sizeToRe'\
  b'cv:\n            '\
  b'        data = s'\
  b'elf._rdBufView\n '\
  b'                '\
  b'   self._rdBufVi'\
  b'ew = None\n      '\
  b'              se'\
  b'lf._asyncSockets'\
  b'Pool.NotifyNextR'\
  b'eadyForReading(s'\
  b'elf, False)\n    '\
  b'                '\
  b'self._removeExpi'\
  b'reTimeout()\n    '\
  b'                '\
  b'if self._onDataR'\
  b'ecv:\n           '\
  b'             try'\
  b':\n              '\
  b'              se'\
  b'lf._onDataRecv(s'\
  b'elf, data, self.'\
  b'_onDataRecvArg)\n'\
  b'                '\
  b'        except E'\
  b'xception as ex:\n'\
  b'                '\
  b'            rais'\
  b'e XAsyncTCPClien'\
  b'tException(\n    '\
  b'                '\
  b"            'Err"\
  b'or when handling'\
  b' the "OnDataRecv'\
  b'" event : %s\' % '\
  b'ex\n             '\
  b'               )'\
  b'\n               '\
  b'     return\n    '\
  b'        else:\n  '\
  b'              re'\
  b'turn\n\n    # ----'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----\n\n    def On'\
  b'ReadyForWriting('\
  b'self):\n        i'\
  b'f self._wrBufVie'\
  b'w:\n            t'\
  b'ry:\n            '\
  b'    n = self._so'\
  b'cket.send(self._'\
  b'wrBufView)\n     '\
  b'       except:\n '\
  b'               r'\
  b'eturn\n          '\
  b'  self._wrBufVie'\
  b'w = self._wrBufV'\
  b'iew[n:]\n        '\
  b'    if not self.'\
  b'_wrBufView:\n    '\
  b'            self'\
  b'._asyncSocketsPo'\
  b'ol.NotifyNextRea'\
  b'dyForWriting(sel'\
  b'f, False)\n      '\
  b'          if sel'\
  b'f._onDataSent:\n '\
  b'                '\
  b'   try:\n        '\
  b'                '\
  b'self._onDataSent'\
  b'(self, self._onD'\
  b'ataSentArg)\n    '\
  b'                '\
  b'except Exception'\
  b' as ex:\n        '\
  b'                '\
  b'raise XAsyncTCPC'\
  b'lientException(\n'\
  b'                '\
  b"            'Err"\
  b'or when handling'\
  b' the "OnDataSent'\
  b'" event : %s\' % '\
  b'ex\n             '\
  b'           )\n\n  '\
  b'  # ------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'------------\n\n  '\
  b'  def AsyncRecvL'\
  b'ine(\n        sel'\
  b'f, lineEncoding='\
  b'"UTF-8", onLineR'\
  b'ecv=None, onLine'\
  b'RecvArg=None, ti'\
  b'meoutSec=None\n  '\
  b'  ):\n        if '\
  b'self._rdLinePos '\
  b'is not None or s'\
  b'elf._sizeToRecv:'\
  b'\n            rai'\
  b'se XAsyncTCPClie'\
  b'ntException(\n   '\
  b'             "As'\
  b'yncRecvLine : Al'\
  b'ready waiting as'\
  b'ynchronous recei'\
  b've."\n           '\
  b' )\n        if se'\
  b'lf._socket:\n    '\
  b'        self._se'\
  b'tExpireTimeout(t'\
  b'imeoutSec)\n     '\
  b'       self._rdL'\
  b'inePos = 0\n     '\
  b'       self._rdL'\
  b'ineEncoding = li'\
  b'neEncoding\n     '\
  b'       self._onD'\
  b'ataRecv = onLine'\
  b'Recv\n           '\
  b' self._onDataRec'\
  b'vArg = onLineRec'\
  b'vArg\n           '\
  b' self._asyncSock'\
  b'etsPool.NotifyNe'\
  b'xtReadyForReadin'\
  b'g(self, True)\n  '\
  b'          return'\
  b' True\n        re'\
  b'turn False\n\n    '\
  b'# --------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------\n\n    '\
  b'def AsyncRecvDat'\
  b'a(\n        self,'\
  b' size=None, onDa'\
  b'taRecv=None, onD'\
  b'ataRecvArg=None,'\
  b' timeoutSec=None'\
  b'\n    ):\n        '\
  b'if self._rdLineP'\
  b'os is not None o'\
  b'r self._sizeToRe'\
  b'cv:\n            '\
  b'raise XAsyncTCPC'\
  b'lientException(\n'\
  b'                '\
  b'"AsyncRecvData :'\
  b' Already waiting'\
  b' asynchronous re'\
  b'ceive."\n        '\
  b'    )\n        if'\
  b' self._socket:\n '\
  b'           if si'\
  b'ze is None:\n    '\
  b'            size'\
  b' = self._recvBuf'\
  b'Slot.Size\n      '\
  b'      elif not i'\
  b'sinstance(size, '\
  b'int) or size <= '\
  b'0:\n             '\
  b'   raise XAsyncT'\
  b'CPClientExceptio'\
  b"n('AsyncRecvData"\
  b' : "size" is inc'\
  b"orrect.')\n      "\
  b'      if size <='\
  b' self._recvBufSl'\
  b'ot.Size:\n       '\
  b'         self._r'\
  b'dBufView = memor'\
  b'yview(self._recv'\
  b'BufSlot.Buffer)['\
  b':size]\n         '\
  b'   else:\n       '\
  b'         try:\n  '\
  b'                '\
  b'  self._rdBufVie'\
  b'w = memoryview(b'\
  b'ytearray(size))\n'\
  b'                '\
  b'except:\n        '\
  b'            rais'\
  b'e XAsyncTCPClien'\
  b'tException(\n    '\
  b'                '\
  b'    "AsyncRecvDa'\
  b'ta : No enought '\
  b'memory to receiv'\
  b'e %s bytes." % s'\
  b'ize\n            '\
  b'        )\n      '\
  b'      self._setE'\
  b'xpireTimeout(tim'\
  b'eoutSec)\n       '\
  b'     self._sizeT'\
  b'oRecv = size\n   '\
  b'         self._o'\
  b'nDataRecv = onDa'\
  b'taRecv\n         '\
  b'   self._onDataR'\
  b'ecvArg = onDataR'\
  b'ecvArg\n         '\
  b'   self._asyncSo'\
  b'cketsPool.Notify'\
  b'NextReadyForRead'\
  b'ing(self, True)\n'\
  b'            retu'\
  b'rn True\n        '\
  b'return False\n\n  '\
  b'  # ------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'------------\n\n  '\
  b'  def AsyncSendD'\
  b'ata(self, data, '\
  b'onDataSent=None,'\
  b' onDataSentArg=N'\
  b'one):\n        if'\
  b' self._socket:\n '\
  b'           try:\n'\
  b'                '\
  b'if bytes([data[0'\
  b']]):\n           '\
  b'         if self'\
  b'._wrBufView:\n   '\
  b'                '\
  b'     self._wrBuf'\
  b'View = memoryvie'\
  b'w(bytes(self._wr'\
  b'BufView) + data)'\
  b'\n               '\
  b'     else:\n     '\
  b'                '\
  b'   self._wrBufVi'\
  b'ew = memoryview('\
  b'data)\n          '\
  b'          self._'\
  b'onDataSent = onD'\
  b'ataSent\n        '\
  b'            self'\
  b'._onDataSentArg '\
  b'= onDataSentArg\n'\
  b'                '\
  b'    self._asyncS'\
  b'ocketsPool.Notif'\
  b'yNextReadyForWri'\
  b'ting(self, True)'\
  b'\n               '\
  b'     return True'\
  b'\n            exc'\
  b'ept Exception as'\
  b' e:\n            '\
  b'    sys.print_ex'\
  b'ception(e)\n     '\
  b'       raise XAs'\
  b'yncTCPClientExce'\
  b"ption('AsyncSend"\
  b'Data : "data" is'\
  b" incorrect.')\n  "\
  b'      return Fal'\
  b'se\n\n    # ------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'--\n\n    def Asyn'\
  b'cSendSendingBuff'\
  b'er(self, size=No'\
  b'ne, onDataSent=N'\
  b'one, onDataSentA'\
  b'rg=None):\n      '\
  b'  if self._wrBuf'\
  b'View:\n          '\
  b'  raise XAsyncTC'\
  b'PClientException'\
  b'(\n              '\
  b'  "AsyncSendBuff'\
  b'erSlot : Already'\
  b' waiting to send'\
  b' data."\n        '\
  b'    )\n        if'\
  b' self._socket:\n '\
  b'           if si'\
  b'ze is None:\n    '\
  b'            size'\
  b' = self._sendBuf'\
  b'Slot.Size\n      '\
  b'      if size > '\
  b'0 and size <= se'\
  b'lf._sendBufSlot.'\
  b'Size:\n          '\
  b'      self._wrBu'\
  b'fView = memoryvi'\
  b'ew(self._sendBuf'\
  b'Slot.Buffer)[:si'\
  b'ze]\n            '\
  b'    self._onData'\
  b'Sent = onDataSen'\
  b't\n              '\
  b'  self._onDataSe'\
  b'ntArg = onDataSe'\
  b'ntArg\n          '\
  b'      self._asyn'\
  b'cSocketsPool.Not'\
  b'ifyNextReadyForW'\
  b'riting(self, Tru'\
  b'e)\n             '\
  b'   return True\n '\
  b'       return Fa'\
  b'lse\n\n    # -----'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'----------------'\
  b'---\n\n    @proper'\
  b'ty\n    def CliAd'\
  b'dr(self):\n      '\
  b'  return self._c'\
  b'liAddr\n\n    @pro'\
  b'perty\n    def Se'\
  b'ndingBuffer(self'\
  b'):\n        retur'\
  b'n self._sendBufS'\
  b'lot.Buffer\n\n    '\
  b'@property\n    de'\
  b'f OnFailsToConne'\
  b'ct(self):\n      '\
  b'  return self._o'\
  b'nFailsToConnect\n'\
  b'\n    @OnFailsToC'\
  b'onnect.setter\n  '\
  b'  def OnFailsToC'\
  b'onnect(self, val'\
  b'ue):\n        sel'\
  b'f._onFailsToConn'\
  b'ect = value\n\n   '\
  b' @property\n    d'\
  b'ef OnConnected(s'\
  b'elf):\n        re'\
  b'turn self._onCon'\
  b'nected\n\n    @OnC'\
  b'onnected.setter\n'\
  b'    def OnConnec'\
  b'ted(self, value)'\
  b':\n        self._'\
  b'onConnected = va'\
  b'lue\n\n\n# ========'\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'====\n# ===( XBuf'\
  b'ferSlot )======='\
  b'================'\
  b'================'\
  b'================'\
  b'===\n# =========='\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'==\n\n\nclass XBuff'\
  b'erSlot:\n    def '\
  b'__init__(self, s'\
  b'ize):\n        se'\
  b'lf._size = size\n'\
  b'        self._bu'\
  b'ffer = bytearray'\
  b'(size)\n\n    @pro'\
  b'perty\n    def Si'\
  b'ze(self):\n      '\
  b'  return self._s'\
  b'ize\n\n    @proper'\
  b'ty\n    def Buffe'\
  b'r(self):\n       '\
  b' return self._bu'\
  b'ffer\n\n\n# ======='\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'=====\n# ========'\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'====\n# ========='\
  b'================'\
  b'================'\
  b'================'\
  b'================'\
  b'===\n'\
  b'')
# /status-code.html size=176
_f3 = const(
  b'<!DOCTYPE html>\n'\
  b'<html lang="en">'\
  b'\n<head>\n    <met'\
  b'a charset="UTF-8'\
  b'">\n    <title>St'\
  b'atus code {code}'\
  b'</title>\n</head>'\
  b'\n<body>\n    <p>S'\
  b'tatus code [{cod'\
  b'e}] {reason}</p>'\
  b'\n</body>\n</html>'\
  b'')
direntries = const(( ( '/http_request.py',  ( _f4, False, 9126 ) ),
 ( '/http_response.py',  ( _f0, False, 17605 ) ),
 ( '/libs',  None ),
 ( '/libs/url_utils.py',  ( _f2, False, 1010 ) ),
 ( '/libs/xasync_sockets.py',  ( _f1, False, 17044 ) ),
 ( '/status-code.html',  ( _f3, False, 176 ) ),
))

version = const(2)
date_frozen = const( '2024/07/08 10:42:11' )

sum_size = const(44961)
files_folders = const(6)
import os
import errno
from io import BytesIO, StringIO
from collections import OrderedDict
import sys
def _get_basename( filename ):
 return filename.split("/")[-1]
def _get_folder( filename ):
 basename = _get_basename( filename )
 folder = filename[0:-len(basename)-1]
 if folder == "":
  folder = "/"
 return folder
class VfsFrozen:
 def __init__( self, direntries, sum_size, files_folders ):    
  self.filedict = OrderedDict( direntries )
  self.sum_size = sum_size
  self.files_folders = files_folders
  self.path = "/"
 def _to_absolute_filename( self, filename ):
  if not filename.startswith( "/" ) :
   filename = self.path + "/" + filename
  if filename.endswith("/") and filename != "/":
   filename = filename[0:-1]
  filename = filename.replace("//", "/")
  parts = filename.split("/")
  i = -1
  while ".." in parts:
   i = parts.index("..")
   if i > 1:
    del parts[i]
    del parts[i-1]
   else:
    self._raise_perm()
  while "." in parts:
   i = parts.index(".")
   del parts[i]
  filename = "".join( "/" + p for p in parts ).replace( "//", "/" )
  return filename
 def _find_file( self, filename ):
  filename = self._to_absolute_filename( filename )
  if filename == "/":
   return None
  if filename in self.filedict:
   return self.filedict[filename]
  else:
   raise OSError( errno.ENOENT )
 def _raise_perm( self ):
  raise OSError(errno.EPERM) # Very common here
 def open( self, filename, mode, buffering=None ):
  for c in mode:
   if c not in "rbt":
    raise OSError( errno.EINVAL )
  dir_entry = self._find_file( filename )
  if dir_entry is None: 
   if filename == "/":
    self._raise_perm()
   raise OSError(errno.EISDIR)
  data = dir_entry[0] # data 
  if not dir_entry[1]: # compressed
   if "b" in mode:
    return BytesIO( data )
   else:
    return StringIO( data )
  else:
   from deflate import DeflateIO, AUTO
   uncompressed_stream = DeflateIO( BytesIO( data ),
       AUTO, 0, True )
   if "b" in mode:
    return uncompressed_stream
   else:
    return StringIO( uncompressed_stream.read() )
 def chdir( self, path ):
  newdir = self._to_absolute_filename( path )
  dir_entry = self._find_file( newdir )
  if dir_entry is None:
   self.path = newdir
   return
  raise OSError( -2 )
 def getcwd( self ):
  if self.path != "/" and self.path.endswith( "/" ):
   return self.path[0:-1]
  return self.path
 def ilistdir( self, path ):
  abspath = self._to_absolute_filename( path ) 
  self._find_file( abspath )
  for filename, dir_entry in self.filedict.items():
   if _get_folder( filename ) == abspath:
    basename = _get_basename( filename )
    if dir_entry is not None:
     yield ( basename, 0x8000, 0,  dir_entry[2] )
    else:
     yield ( basename, 0x4000, 0, 0 )
 def stat( self, filename ): 
  dir_entry = self._find_file( filename )
  if dir_entry is None:
   return (0x4000, 0,0,0,0,0, 0, 0,0,0)
  else:
   return (0x8000, 0,0,0,0,0, dir_entry[2], 0,0,0)
 def statvfs( self, *args ):
  return (1,1,self.sum_size,  0,0,self.files_folders,  0,0,1, 255)
 def mount( self, readonly, x ):
  self.path = "/"
 def remove( self, filename ):
  self._raise_perm()
 def mkdir( self, *args ):
  self._raise_perm()
 def rename( self, oldfname, newfname ):
  self._raise_perm()
 def umount( self ):
  pass
 def rmdir(self):
  self._raise_perm()
def mount_fs( frozen_module_name, target, silent ):
 if target is None:
  raise ValueError("No target specified")
 file_exists = False
 try:
  os.stat( target )
  file_exists = True
 except:
  pass  
 if file_exists:
  raise OSError( errno.EEXIST )
 if not silent:
  print( f"mounting {__name__} at {target}." )
 os.mount( VfsFrozen( direntries, sum_size, files_folders ), target, readonly=True )
 return True

mount_fs( __name__, '/micro_web_srv_2', False )