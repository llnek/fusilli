;; This library is distributed in  the hope that it will be useful but without
;; any  warranty; without  even  the  implied  warranty of  merchantability or
;; fitness for a particular purpose.
;; The use and distribution terms for this software are covered by the Eclipse
;; Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
;; can be found in the file epl-v10.html at the root of this distribution.
;; By using this software in any  fashion, you are agreeing to be bound by the
;; terms of this license. You  must not remove this notice, or any other, from
;; this software.
;; Copyright (c) 2013-2015, Ken Leung. All rights reserved.

(set-env!

  :buildVersion "0.9.0-SNAPSHOT"
  :buildDebug true
  :buildType "cpp"

  :DOMAIN "czlab.fusilli"
  :PID "fusilli"

  :source-paths #{"src/main/cpp"}

  :dependencies '[ ] )

(require '[czlab.tpcl.boot :as b :refer [fp! ge ]]
         '[clojure.data.json :as js]
         '[clojure.java.io :as io]
         '[clojure.string :as cs]
         '[boot.core :as bc]
         '[czlab.tpcl.antlib :as a])

(import '[java.io File])

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(b/BootEnvVars)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(b/BootEnvPaths)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(defn- compileJS ""

  [wappid]

  (let [root (io/file (ge :webDir) wappid "src")
        tks (atom []) ]
    (try
      (cleanLocalJs wappid)
      (b/BabelTree root (partial onbabel wappid))
    (finally
      (cleanLocalJs wappid)))

    (when false
      (a/RunTasks*
        (a/AntExec
          {:executable "jsdoc"
           :dir (ge :basedir)
           :spawn true}
          [[:argvalues [(fp! (ge :websrc) wappid)
                         "-c"
                         (fp! (ge :basedir)
                              "jsdoc.json")
                         "-d"
                         (fp! (ge :docs) wappid)]]])))

    (.mkdirs (io/file (ge :basedir) "public/ig/lib/game"))
    (.mkdirs (io/file (ge :basedir) "public/scripts"))

    (case wappid

      "cocos2d"
      (->> (a/AntCopy
             {:todir (fp! (ge :basedir) "public/ig/lib") }
             [[:fileset {:dir (fp! (ge :websrc) wappid)}]])
           (conj @tks)
           (reset! tks))

      "main"
      (->> (a/AntCopy
             {:todir (fp! (ge :basedir) "public/scripts") }
             [[:fileset {:dir (fp! (ge :websrc) wappid) } ]])
           (conj @tks)
           (reset! tks))
      ;;else
      (do
        (->> (a/AntCopy
               {:file (fp! (ge :srcDir) "resources/main.js")
                :todir (fp! (ge :basedir)
                            "public/ig/lib/game" wappid)} )
             (conj @tks)
             (reset! tks))
        (->> (a/AntCopy
               {:todir (fp! (ge :basedir)
                            "public/ig/lib/game" wappid)}
               [[:fileset {:dir (fp! (ge :websrc) wappid)} ]])
             (conj @tks)
             (reset! tks))))

    (apply a/RunTasks* (reverse @tks))
  ))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(defn- compileMedia ""

  [wappid]

  (a/RunTasks*
    (a/AntMkdir {:dir (fp! (ge :basedir)
                           "public/ig/res" wappid)})
    (a/AntCopy
      {:todir (fp! (ge :basedir)
                   "public/ig/res" wappid)}
      [[:fileset {:dir (fp! (ge :webDir) wappid "res/sd")}]
       [:fileset {:dir (fp! (ge :webDir) wappid "res")
                  :includes "sfx/**/*"}]])))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(defn- compileInfo ""

  [wappid]

  (when-not (or (= "cocos2d" wappid)
                (= "main" wappid))
    (a/RunTasks*
      (a/AntMkdir {:dir (fp! (ge :basedir)
                             "public/ig/info" wappid)})
      (a/AntCopy
        {:todir (fp! (ge :basedir) "public/ig/info" wappid)}
        [[:fileset {:dir (fp! (ge :webDir) wappid "info")}]]))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(defn- compilePages ""

  [wappid]

  (case wappid
    ("main" "cocos2d")
    (a/RunTasks*
      (a/AntCopy
        {:todir (fp! (ge :basedir) "public/pages" wappid)}
        [[:fileset {:dir (fp! (ge :webDir) wappid "pages")}]]))
    ;;else
    (jiggleTheIndexFile wappid
                        (fp! (ge :basedir)
                             "public/pages" wappid)
                        false)
  ))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(defn- finzApp ""

  [wappid]

  (when (not= (ge :pmode) "release")
    (let [des (fp! (ge :basedir)
                   "public/ig/lib/game" wappid)]
      (a/CopyFile (fp! (ge :srcDir) "resources/project.json") des)
      (a/CopyFile (fp! (ge :srcDir) "resources/main.js") des))
  ))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(defn- buildOneWebApp ""

  [^File dir]

  (let [wappid (.getName dir)]
    (.mkdirs (io/file (fp! (ge :websrc) wappid)))
    (.mkdirs (io/file (fp! (ge :webcss) wappid)))
    (doto wappid
      (compileJS)
      (compileSCSS)
      (compileMedia)
      (compileInfo)
      (compilePages)
      (finzApp))
  ))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(defn- buildWebApps ""

  []

  (let [isDir? #(.isDirectory %)
        dirs (->> (.listFiles (io/file (ge :webDir)))
                  (filter isDir?))]
    (doall (map #(buildOneWebApp %) dirs))
  ))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; task definitions ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(deftask clean4build "clean,pre-build"

  []

  (bc/with-pre-wrap fileset
    (b/Clean4Build)
    (b/PreBuild)
    fileset))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(deftask dev "clean,resolve,build"

  []

  (comp (clean4build)
        (b/libjars)
        (b/buildr)))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(deftask games ""

  []

  (bc/with-pre-wrap fileset
    (b/CleanPublic)
    (fn [& args]
      (a/CleanDir (fp! (ge :basedir) "public/ig"))
      (a/CleanDir (fp! (ge :websrc)))
      (a/CleanDir (fp! (ge :webcss))))
    (buildWebApps)
    fileset))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(deftask rel ""

  []

  (bc/with-pre-wrap fileset
    (b/CleanPublic)
    (fn [& args]
      (a/CleanDir (fp! (ge :basedir) "public/ig"))
      (a/CleanDir (fp! (ge :websrc)))
      (a/CleanDir (fp! (ge :webcss))))
    (buildWebApps)
    (yuiCSS)
    (yuiJS)
    (finzBuild)
    fileset))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(deftask release ""
  []

  (set-env! :pmode "release")
  (comp (dev) (rel)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(defn- mkGame ""

  [appid]

  (let [pubtime (b/FmtTime "yyyy-MM-dd")
        appkey (b/RandUUID)]

    ;;(format "Creating new game: %s" appid)

    (doseq [s ["src/n" "src/s" "src/p"
               "src/i18n" "res/sfx"]]
      (.mkdirs (io/file (fp! (ge :webDir) appid s))))

    (doseq [s ["hdr" "hds" "sd"]]
      (.mkdirs (io/file (ge :webDir) appid "res" s "pics"))
      (.mkdirs (io/file (ge :webDir) appid "res" s "fon")))

    (doseq [s ["info" "pages" "styles"]]
      (.mkdirs (io/file (ge :webDir) appid s)))

    (a/CopyFile (fp! (ge :srcDir) "resources/game.json")
                (io/file (ge :webDir) appid "info"))
    (a/CopyFile (fp! (ge :srcDir) "resources/game.mf")
                (io/file (ge :webDir) appid "info"))

    (b/ReplaceFile (fp! (ge :webDir) appid "info/game.mf")
                   #(-> (cs/replace % "@@PUBDATE@@" pubtime)
                        (cs/replace "@@APPID@@" appid)))

    (b/ReplaceFile (fp! (ge :webDir) appid "info/game.json")
                   #(cs/replace % "@@UUID@@" appkey))

    (doseq [s ["game.js" "splash.js" "mmenu.js"
               "hud.js" "config.js" "protos.js"]]
      (a/CopyFile (fp! (ge :srcDir) "resources" s)
                  (fp! (ge :webDir) appid "src/p")))

    (a/CopyFile (fp! (ge :srcDir) "resources/gnodes.js")
                (fp! (ge :webDir) appid "src/n"))

    (a/CopyFile (fp! (ge :srcDir) "resources/cobjs.js")
                (fp! (ge :webDir) appid "src/n"))

    (doseq [s ["stager.js" "factory.js"
               "motion.js" "resolve.js" "sysobjs.js"]]
      (a/CopyFile (io/file (ge :srcDir) "resources" s)
                  (fp! (ge :webDir) appid "src/s")))

    (b/ReplaceFile (fp! (ge :webDir)
                        appid "src/p/config.js")
                   #(cs/replace % "@@UUID@@" appkey))

    (a/CopyFile (fp! (ge :srcDir) "resources/l10n.js")
                (fp! (ge :webDir) appid "src/i18n/l10n.js"))

    (a/CopyFile (fp! (ge :srcDir) "resources/ccconfig.js")
                (fp! (ge :webDir) appid "src"))

    (a/CopyFile (fp! (ge :srcDir) "resources/proj.json")
                (fp! (ge :webDir) appid "src/project.json"))

    (b/ReplaceFile (fp! (ge :webDir) appid "src/project.json")
                   #(cs/replace % "@@APPID@@" appid))

    (b/ReplaceFile (fp! (ge :webDir) appid "src/ccconfig.js")
                   #(cs/replace % "@@APPID@@" appid))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(deftask newgame

  ""
  [n id VAL str "game id"]

  (bc/with-pre-wrap fileset
    (mkGame id)
    fileset))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(defn- cocos->new ""

  [appid]

  (a/RunTasks*
    (a/AntMkdir {:dir (fp! (ge :basedir) "cocos")})
    (a/AntExec
      {:executable "cocos"}
      [[:argvalues ["new" "-l" "js" "-t"
                    "runtime" "--ios-bundleid"
                    (str "com.zotohlab.p." appid)
                    "-d" (fp! (ge :basedir) "cocos") appid]]])))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(deftask cocos+new

  ""
  [n id VAL str "game id"]

  (bc/with-pre-wrap fileset
    (cocos->new id)
    fileset))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(defn- deploy->app ""

  [appid]

  (let [vendors (fp! (ge :basedir) "public/vendors")
        despath (fp! (ge :basedir) "cocos" appid)
        srcpath (fp! (ge :webDir) appid)
        des (io/file despath)
        src (io/file srcpath)
        dd2 (io/file des "res")
        dd1 (io/file des "src") ]

    (when-not (.exists des) (cocos->new appid))
    ;;(print (format "Deploying game: %s\n" appid))
    (a/CleanDir dd2)
    (a/CleanDir dd1)
    ;; resources
    (doseq [s ["hdr" "hds" "sd" "sfx"]]
      (a/RunTasks*
        (a/AntCopy
          {:todir (io/file dd2 s "cocos2d") }
          [[:fileset {:dir (fp! (ge :webDir) "cocos2d/res" s)} ]])
        (a/AntCopy
          {:todir (io/file dd2 s appid)}
          [[:fileset {:dir (io/file (ge :srcpath) "res" s)} ]])))
    ;; js code
    (a/RunTasks*
      (a/AntCopy
        {:todir (io/file dd1 "zotohlab")}
        [[:fileset {:dir (fp! (ge :webDir) "cocos2d/src/zotohlab")} ]])
      (a/AntCopy
        {:todir (io/file dd1 appid)}
        [[:fileset {:dir (fp! srcpath "src")} ]])
      (a/AntCopy
        {:todir (io/file dd1 "helpers")}
        [[:fileset {:dir (fp! vendors "helpers")
                    :includes "dbg.js"}]]))

    (doseq [s ["almond" "js-signals" "ash-js"
               "rxjs" "ramda" "cherimoia"
               "l10njs" "cookies" "mustache"]]
      (a/RunTasks*
        (a/AntCopy
          {:todir (io/file dd1  s)}
          [[:fileset {:dir (io/file vendors s)} ]])))

    ;; boot stuff
    (a/RunTasks*
      (a/AntCopy
        {:todir despath
         :overwrite true}
        [[:fileset {:dir (io/file (ge :srcDir) "resources")
                    :includes (str "manifest.webapp,project.json,"
                                   "main.js,index.html")}]]))

    (let [j1 (-> (slurp (fp! (ge :srcDir)
                             "resources/project.json")
                        :encoding "utf-8")
                 (js/read-str :key-fn keyword))
          jm (-> (slurp (io/file despath "manifest.webapp")
                        :encoding "utf-8"))
          j2 (-> (slurp (fp! srcpath "src/project.json")
                        :encoding "utf-8")
                 (js/read-str :key-fn keyword)) ]
      (->> (-> jm
              (cs/replace "@@APP@@" appid)
              (cs/replace "@@USER@@" (System/getProperty "user.name")))
           (spit (io/file despath "manifest.webapp")))

      (->> (update-in j1
                      [:jsList]
                      #(concat % (:jsList j2)))
           (js/write-str )
           (spit (io/file despath "project.json"))))

    (jiggleTheIndexFile appid
                        (io/file (ge :basedir) "cocos" appid) true)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(deftask deployapp

  ""
  [n id VAL str "game id"]

  (bc/with-pre-wrap fileset
    (let [srcpath (fp! (ge :webDir) id)
          src (io/file srcpath)]
      (if-not (.exists src)
        (print (format "Invalid game: %s\n" id))
        (deploy->app id)))
    fileset))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;EOF
