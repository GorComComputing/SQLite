describe 'database' do
    def run_script(commands)
      raw_output = nil
      IO.popen("./SQLite", "r+") do |pipe|
        commands.each do |command|
          pipe.puts command
        end
  
        pipe.close_write
  
        # Read entire output
        raw_output = pipe.gets(nil)
      end
      raw_output.split("\n")
    end
  
    it 'inserts and retrieves a row' do
      result = run_script([
        "insert 1 user1 person1@example.com",
        "select",
        ".exit",
      ])
      expect(result).to match_array([
        "SQLite Gor.Com",
        "Started",
        "db > Executed.",
        "db > (1, user1, person1@example.com)",
        "Executed.",
        "db > ",
      ])
    end


    it 'prints error message when table is full' do
      script = (2..101).map do |i|
        "insert #{i} user#{i} person#{i}@example.com"
        #puts "insert #{i} user#{i} person#{i}@example.com"
        #puts "Ended Two..."
      end
      script << ".exit"
      
      result = run_script(script)

      (2..101).map do |i|
        puts " #{i} - #{result[i]} "
      end

      expect(result[-2]).to eq("db > Error: Table full.")
    end


    it 'allows inserting strings that are the maximum length' do
      long_username = "a"*32
      long_email = "s"*255
      script = [
        "insert 1 #{long_username} #{long_email}",
        "select",
        ".exit",
      ]
      result = run_script(script)
      expect(result).to match_array([
        "SQLite Gor.Com",
        "Started",
        "db > Executed.",
        "db > (1, #{long_username}, #{long_email})",
        "Executed.",
        "db > ",
      ])
    end


    it 'prints error message if strings are too long' do
      long_username = "a"*33
      long_email = "s"*256
      script = [
        "insert 1 #{long_username} #{long_email}",
        "select",
        ".exit",
      ]
      result = run_script(script)
      expect(result).to match_array([
        "SQLite Gor.Com",
        "Started",
        "db > String is too long.",
        "Executed.",
        "db > ",
      ])
    end




  end